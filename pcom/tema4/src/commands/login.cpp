#include "../../include/commands/login.h"

void LoginCommand::execute() const {
	if (!Client::cookies.empty()) {
		std::cerr << "ERROR: You are already logged in!" << std::endl;
		return;
	}

	std::string username;
	std::cout << "username=";
	std::getline(std::cin, username);

	std::string password;
	std::cout << "password=";
	std::getline(std::cin, password);

	if (username.empty()) {
		std::cerr << "ERROR: The username must not be empty!" << std::endl;
		return;
	}

	if (username.find(' ') != std::string::npos) {
		std::cerr << "ERROR: The username must not contain spaces!" << std::endl;
		return;
	}

	if (password.empty()) {
		std::cerr << "ERROR: The password must not be empty!" << std::endl;
		return;
	}

	if (password.find(' ') != std::string::npos) {
		std::cerr << "ERROR: The password must not contain spaces!" << std::endl;
		return;
	}

	nlohmann::json user;
	user["username"] = username;
	user["password"] = password;

	Request::method = "POST";
	Request::route = "/api/v1/tema/auth/login";
	Request::body = user;

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
		std::cerr << "ERROR: The username or password is not valid!" << std::endl;
		return;
	}

	Client::cookies.clear();
	Utils::add_cookie(response, "connect.sid");
	std::cout << "SUCCESS: Logged in successfully!" << std::endl;
}
