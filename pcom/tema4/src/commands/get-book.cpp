#include "../../include/commands/get-book.h"

void GetBookCommand::execute() const {
	if (Client::token.empty()) {
		std::cerr << "ERROR: You do not have access to the library!" << std::endl;
		return;
	}

	std::string id;
	std::cout << "id=";
	std::getline(std::cin, id);

	if (id.empty()) {
		std::cerr << "ERROR: The id must not be empty!" << std::endl;
		return;
	}

	if (strtol(id.c_str(), nullptr, 10) == 0) {
		std::cerr << "ERROR: The id must be a positive integer!" << std::endl;
		return;
	}

	Request::method = "GET";
	Request::route = "/api/v1/tema/library/books/" + id;
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
		std::cerr << "ERROR: Something went wrong reading the book!" << std::endl;
		return;
	}

	std::string book_string = response.substr(response.find("{\""));
	nlohmann::json book = nlohmann::json::parse(book_string);
	std::cout << book.dump(4) << std::endl;
}
