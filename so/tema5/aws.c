#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <sys/eventfd.h>
#include <libaio.h>
#include <errno.h>

#include "aws.h"
#include "utils/util.h"
#include "utils/debug.h"
#include "utils/sock_util.h"
#include "utils/w_epoll.h"

static int listenfd;

static int epollfd;

static int aws_on_path_cb(http_parser *p, const char *buf, size_t len) {
	struct connection *conn = p->data;

	memcpy(conn->request_path, buf, len);
	conn->request_path[len] = '\0';
	conn->have_path = 1;

	return 0;
}

static void connection_prepare_send_reply_header(struct connection *conn) {
	struct stat buf;

	if (fstat(conn->fd, &buf) < 0) {
		ERR("fstat failed\n");
		exit(1);
	}

	conn->file_size = buf.st_size;

	snprintf(conn->send_buffer, sizeof(conn->send_buffer),
	         "HTTP/1.0 200 OK\r\n"
	         "Date: Sun, 08 May 2011 09:26:16 GMT\r\n"
	         "Server: Apache/2.2.9\r\n"
	         "Last-Modified: Mon, 02 Aug 2010 17:55:28 GMT\r\n"
	         "Accept-Ranges: bytes\r\n"
	         "Vary: Accept-Encoding\r\n"
	         "Connection: close\r\n"
	         "Content-Type: text/html\r\n"
	         "Content-Length: %ld\r\n\r\n",
	         conn->file_size);

	conn->send_len = strlen(conn->send_buffer);
}

static void connection_prepare_send_404(struct connection *conn) {
	memcpy(conn->send_buffer, "HTTP/1.1 404 Not Found\r\n\r\n", 26);
	conn->send_len = strlen(conn->send_buffer);
}

static enum resource_type connection_get_resource_type(struct connection *conn) {
	enum resource_type res_type = RESOURCE_TYPE_NONE;

	char request_path[BUFSIZ + 3];

	snprintf(request_path, sizeof(request_path), "%s%s", AWS_DOCUMENT_ROOT, conn->request_path);

	if (strstr(request_path, AWS_REL_STATIC_FOLDER) != NULL)
		res_type = RESOURCE_TYPE_STATIC;

	else if (strstr(request_path, AWS_REL_DYNAMIC_FOLDER) != NULL)
		res_type = RESOURCE_TYPE_DYNAMIC;

	return res_type;
}

struct connection *connection_create(int sockfd) {
	struct connection *conn = calloc(1, sizeof(struct connection));

	if (conn == NULL) {
		ERR("calloc failed\n");
		exit(1);
	}

	conn->fd = -1;
	conn->eventfd = -1;
	conn->sockfd = sockfd;

	return conn;
}

void connection_start_async_io(struct connection *conn) {
	char *large_send_buffer = calloc(conn->file_size, sizeof(char));

	conn->eventfd = eventfd(0, 0);
	io_prep_pread(&conn->iocb, conn->fd, large_send_buffer, conn->file_size, 0);
	io_set_eventfd(&conn->iocb, conn->eventfd);

	conn->iocb.data = large_send_buffer;
	conn->piocb[0] = &conn->iocb;

	if (w_epoll_add_ptr_in(epollfd, conn->eventfd, conn) < 0) {
		ERR("w_epoll_add_ptr_in failed\n");
		exit(1);
	}

	size_t nr_events = 1;

	io_setup(nr_events, &conn->ctx);
	conn->state = STATE_ASYNC_ONGOING;
	io_submit(conn->ctx, nr_events, conn->piocb);
}

void connection_remove(struct connection *conn) {
	if (w_epoll_remove_ptr(epollfd, conn->sockfd, conn) < 0) {
		ERR("w_epoll_remove_ptr failed\n");
		exit(1);
	}

	close(conn->fd);
	close(conn->eventfd);
	tcp_close_connection(conn->sockfd);
	free(conn);
}

void handle_new_connection(void) {
	struct sockaddr address;
	socklen_t address_len = sizeof(struct sockaddr);

	int sockfd = accept(listenfd, &address, &address_len);

	if (sockfd < 0) {
		ERR("accept failed\n");
		exit(1);
	}

	if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK) < 0) {
		ERR("fcntl failed\n");
		exit(1);
	}

	struct connection *conn = connection_create(sockfd);

	if (w_epoll_add_ptr_in(epollfd, sockfd, conn) < 0) {
		ERR("w_epoll_add_ptr_in failed\n");
		exit(1);
	}

	http_parser_init(&conn->request_parser, HTTP_REQUEST);
	conn->request_parser.data = conn;
}

void receive_data(struct connection *conn) {
	ssize_t size_recv = recv(conn->sockfd, conn->recv_buffer + conn->recv_len, 256, MSG_DONTWAIT);

	if (size_recv <= 0) {
		ERR("recv failed\n");
		exit(1);
	}

	conn->recv_len = conn->recv_len + size_recv;

	if (strstr(conn->recv_buffer, "\r\n\r\n") == NULL)
		return;

	parse_header(conn);
}

int connection_open_file(struct connection *conn) {
	if (w_epoll_update_ptr_inout(epollfd, conn->sockfd, conn) < 0) {
		ERR("w_epoll_update_ptr_inout failed\n");
		exit(1);
	}

	char request_path[BUFSIZ + 3];

	snprintf(request_path, sizeof(request_path), "%s%s", AWS_DOCUMENT_ROOT, conn->request_path);
	conn->fd = open(request_path, O_RDWR);

	if (conn->fd < 0) {
		connection_prepare_send_404(conn);
		return STATE_SENDING_404;
	}

	connection_prepare_send_reply_header(conn);
	return STATE_SENDING_HEADER;
}

void connection_complete_async_io(struct connection *conn) {
	if (read(conn->eventfd, &conn->async_read_len, sizeof(size_t)) < 0) {
		ERR("read failed\n");
		exit(1);
	}

	if (w_epoll_update_ptr_inout(epollfd, conn->eventfd, conn) < 0) {
		ERR("w_epoll_update_ptr_inout failed\n");
		exit(1);
	}

	conn->state = STATE_SENDING_DATA;
}

int parse_header(struct connection *conn) {
	http_parser_settings settings_on_path = {
			.on_message_begin = 0,
			.on_header_field = 0,
			.on_header_value = 0,
			.on_path = aws_on_path_cb,
			.on_url = 0,
			.on_fragment = 0,
			.on_query_string = 0,
			.on_body = 0,
			.on_headers_complete = 0,
			.on_message_complete = 0
	};

	http_parser_execute(&conn->request_parser, &settings_on_path, conn->recv_buffer, conn->recv_len);
	conn->res_type = connection_get_resource_type(conn);
	conn->state = connection_open_file(conn);

	return 0;
}

enum connection_state connection_send_static(struct connection *conn) {
	while (conn->file_size > 0) {
		ssize_t size_sendfile = sendfile(conn->sockfd, conn->fd, NULL, conn->file_size);

		if (size_sendfile == 0)
			break;

		conn->file_size = conn->file_size - size_sendfile;
	}

	return conn->file_size == 0 ? STATE_CONNECTION_CLOSED : STATE_DATA_SENT;
}

int connection_send_data(struct connection *conn) {
	while (conn->send_len > 0) {
		ssize_t size_send = send(conn->sockfd, conn->send_buffer, conn->send_len, MSG_DONTWAIT);

		if (size_send < 0)
			return -1;

		if (size_send == 0)
			break;

		conn->send_len = conn->send_len - size_send;
		memcpy(conn->send_buffer, conn->send_buffer + size_send, conn->send_len);
	}

	if (conn->state == STATE_SENDING_HEADER)
		conn->state = STATE_HEADER_SENT;

	if (conn->state == STATE_SENDING_404)
		conn->state = STATE_404_SENT;

	return 0;
}

int connection_send_dynamic(struct connection *conn) {
	if (conn->eventfd < 0)
		connection_start_async_io(conn);

	if (w_epoll_update_ptr_in(epollfd, conn->sockfd, conn) < 0)
		return -1;

	return 0;
}

void handle_input(struct connection *conn) {
	switch (conn->state) {
		case STATE_INITIAL:
		case STATE_RECEIVING_DATA:
			receive_data(conn);
			break;

		case STATE_ASYNC_ONGOING:
			connection_complete_async_io(conn);
			break;

		default:
			connection_remove(conn);
			break;
	}
}

void handle_output(struct connection *conn) {
	switch (conn->state) {
		case STATE_SENDING_HEADER:
			if (connection_send_data(conn) < 0) {
				ERR("connection_send_data failed\n");
				exit(1);
			}

			break;

		case STATE_HEADER_SENT:
			if (conn->res_type == RESOURCE_TYPE_DYNAMIC) {
				if (connection_send_dynamic(conn) < 0) {
					ERR("connection_send_dynamic failed\n");
					exit(1);
				}

				break;
			}

			conn->state = connection_send_static(conn);
			break;

		case STATE_SENDING_DATA:
			if (conn->file_size == 0) {
				conn->state = STATE_DATA_SENT;
				break;
			}

			size_t size_send = send(conn->sockfd, conn->iocb.data, conn->file_size, MSG_DONTWAIT);

			conn->file_size = conn->file_size - size_send;
			memcpy(conn->iocb.data, conn->iocb.data + size_send, conn->file_size);
			break;

		case STATE_SENDING_404:
			if (connection_send_data(conn) < 0) {
				ERR("connection_send_data failed\n");
				exit(1);
			}

			break;

		case STATE_404_SENT:
			if (conn->fd < 0) {
				if (w_epoll_update_ptr_in(epollfd, conn->sockfd, conn) < 0) {
					ERR("w_epoll_update_ptr_in failed\n");
					exit(1);
				}

				conn->state = STATE_INITIAL;
				break;
			}

			if (conn->res_type == RESOURCE_TYPE_DYNAMIC) {
				if (connection_send_dynamic(conn) < 0) {
					ERR("connection_send_dynamic failed\n");
					exit(1);
				}

				break;
			}

			conn->state = connection_send_static(conn);
			break;

		default:
			connection_remove(conn);
			break;
	}
}

int main(void) {
	epollfd = w_epoll_create();

	if (epollfd < 0) {
		ERR("w_epoll_create failed\n");
		exit(1);
	}

	listenfd = tcp_create_listener(AWS_LISTEN_PORT, DEFAULT_LISTEN_BACKLOG);

	if (listenfd < 0) {
		ERR("tcp_create_listener failed\n");
		exit(1);
	}

	if (w_epoll_add_fd_in(epollfd, listenfd) < 0) {
		ERR("w_epoll_add_fd_in failed\n");
		exit(1);
	}

	while (1) {
		struct epoll_event rev;

		if (w_epoll_wait_infinite(epollfd, &rev) < 0) {
			ERR("w_epoll_wait_infinite failed\n");
			exit(1);
		}

		if (rev.data.fd != listenfd) {
			if (rev.events & EPOLLOUT)
				handle_output(rev.data.ptr);

			else if (rev.events & EPOLLIN)
				handle_input(rev.data.ptr);

			continue;
		}

		if (rev.events & EPOLLIN)
			handle_new_connection();
	}

	close(listenfd);
	close(epollfd);

	return 0;
}
