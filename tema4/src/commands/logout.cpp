#include "../../include/commands/logout.h"

void LogoutCommand::execute() const {
	if (Client::token.empty()) {
		std::cerr << "ERROR: You are not logged in!" << std::endl;
		return;
	}

	Request::method = "GET";
	Request::route = "/api/v1/tema/auth/logout";
	Request::body = nullptr;

	const std::string request = Request::compute();
	Utils::send_to_server(request);
	std::string response = Utils::receive_from_server();

	if (response.empty()) {
		Client::socket_fd = Utils::open_connection();
		Utils::send_to_server(request);
		response = Utils::receive_from_server();
	}

	int status_code = Utils::get_status_code(response);

	if (status_code < 200 || status_code >= 300) {
		std::cerr << "ERROR: Something went wrong logging out!" << std::endl;
		return;
	}

	Client::token.clear();
	Client::cookies.clear();
	std::cout << "SUCCESS: Logged out successfully!" << std::endl;
}
