#include "../include/utils.h"

const std::string Utils::LINE_SEPARATOR = "\r\n";
const std::string Utils::IP = "34.246.184.49";
const int Utils::PORT = 8080;

int Utils::open_connection() {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0) {
		std::cerr << "ERROR: Something went wrong opening the socket connection!" << std::endl;
		exit(0);
	}

	struct sockaddr_in server_address{};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(Utils::PORT);
	inet_aton(Utils::IP.c_str(), &server_address.sin_addr);

	if (connect(socket_fd, (const sockaddr *) &server_address, sizeof(server_address)) < 0) {
		std::cerr << "ERROR: Something went wrong connecting to the server!" << std::endl;
		exit(0);
	}

	return socket_fd;
}

void Utils::send_to_server(const std::string &request) {
	ssize_t bytes_sent;
	size_t total_bytes_sent = 0;

	while (total_bytes_sent < request.size()) {
		bytes_sent = write(Client::socket_fd,
		                   request.c_str() + total_bytes_sent,
		                   request.size() - total_bytes_sent);

		if (bytes_sent < 0) {
			std::cerr << "ERROR: Something went wrong reading from the socket!" << std::endl;
			exit(0);
		}

		if (bytes_sent == 0)
			break;

		total_bytes_sent = total_bytes_sent + bytes_sent;
	}
}

std::string Utils::receive_from_server() {
	std::string response;
	size_t header_end_position = 0;
	size_t body_length = 0;

	while (true) {
		char buffer[4096];
		memset(buffer, 0, sizeof(buffer));
		ssize_t bytes_read = read(Client::socket_fd, buffer, 4096);

		if (bytes_read < 0) {
			std::cerr << "ERROR: Something went wrong reading from the socket!" << std::endl;
			exit(0);
		}

		if (bytes_read == 0)
			break;

		response.append(buffer, bytes_read);
		header_end_position = response.find(Utils::LINE_SEPARATOR + Utils::LINE_SEPARATOR);

		if (header_end_position != std::string::npos) {
			size_t body_length_start_position = response.find("Content-Length: ");
			header_end_position = header_end_position + Utils::LINE_SEPARATOR.size() * 2;

			if (body_length_start_position == std::string::npos)
				continue;

			body_length_start_position = body_length_start_position + 16;
			body_length = strtol(response.data() + body_length_start_position, nullptr, 10);
			break;
		}
	}

	while (response.size() < header_end_position + body_length) {
		char buffer[4096];
		memset(buffer, 0, sizeof(buffer));
		ssize_t bytes_read = read(Client::socket_fd, buffer, 4096);

		if (bytes_read < 0) {
			std::cerr << "ERROR: Something went wrong reading from the socket!" << std::endl;
			exit(0);
		}

		if (bytes_read == 0)
			break;

		response.append(buffer, bytes_read);
	}

	if (response.empty())
		return response;

	return response.substr(response.find("HTTP/1.1"));
}

int Utils::get_status_code(const std::string &response) {
	std::string status_code = response.substr(9, 3);
	return std::stoi(status_code);
}

void Utils::add_cookie(const std::string &response, const std::string &cookie_name) {
	size_t cookie_start_position = response.find(cookie_name + "=");
	size_t cookie_end_position = response.find(Utils::LINE_SEPARATOR, cookie_start_position);
	std::string cookie = response.substr(cookie_start_position, cookie_end_position - cookie_start_position);
	Client::cookies.push_back(cookie);
}

void Utils::get_token(const std::string &response) {
	size_t token_start_position = response.find(R"({"token":")") + 10;
	size_t token_end_position = response.find('\"', token_start_position);
	std::string token = response.substr(token_start_position, token_end_position - token_start_position);
	Client::token = token;
}

void Utils::format_field(std::string &field) {
	std::string formatted_field;

	for (char &character: field)
		if (character != '_')
			formatted_field.push_back(character);

	field = formatted_field;
}
