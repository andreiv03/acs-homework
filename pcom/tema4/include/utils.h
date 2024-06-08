#ifndef UTILS_H
#define UTILS_H

#include "client.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Utils {
public:
	static const std::string LINE_SEPARATOR;
	static const std::string IP;
	static const int PORT;

	static int open_connection();

	static void send_to_server(const std::string &request);

	static std::string receive_from_server();

	static int get_status_code(const std::string &response);

	static void add_cookie(const std::string &response, const std::string &cookie_name);

	static void get_token(const std::string &response);

	static void format_field(std::string &field);
};

#endif