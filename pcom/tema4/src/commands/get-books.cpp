#include "../../include/commands/get-books.h"

void GetBooksCommand::execute() const {
	if (Client::token.empty()) {
		std::cerr << "ERROR: You do not have access to the library!" << std::endl;
		return;
	}

	Request::method = "GET";
	Request::route = "/api/v1/tema/library/books";
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
		std::cerr << "ERROR: Something went wrong reading the books!" << std::endl;
		return;
	}

	if (response.find("[{\"") == std::string::npos) {
		std::cerr << "ERROR: No books found!" << std::endl;
		return;
	}

	std::string books_string = response.substr(response.find("[{\""));
	nlohmann::json books = nlohmann::json::parse(books_string);
	std::cout << books.dump(4) << std::endl;
}
