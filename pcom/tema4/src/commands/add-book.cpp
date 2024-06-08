#include "../../include/commands/add-book.h"

void AddBookCommand::execute() const {
	if (Client::token.empty()) {
		std::cerr << "ERROR: You do not have access to the library!" << std::endl;
		return;
	}

	std::string title;
	std::cout << "title=";
	std::getline(std::cin, title);
	Utils::format_field(title);

	std::string author;
	std::cout << "author=";
	std::getline(std::cin, author);
	Utils::format_field(author);

	std::string genre;
	std::cout << "genre=";
	std::getline(std::cin, genre);
	Utils::format_field(genre);

	std::string publisher;
	std::cout << "publisher=";
	std::getline(std::cin, publisher);
	Utils::format_field(publisher);

	std::string page_count;
	std::cout << "page_count=";
	std::getline(std::cin, page_count);

	if (title.empty()) {
		std::cerr << "ERROR: The title must not be empty!" << std::endl;
		return;
	}

	if (author.empty()) {
		std::cerr << "ERROR: The author must not be empty!" << std::endl;
		return;
	}

	if (genre.empty()) {
		std::cerr << "ERROR: The genre must not be empty!" << std::endl;
		return;
	}

	if (publisher.empty()) {
		std::cerr << "ERROR: The publisher must not be empty!" << std::endl;
		return;
	}

	if (page_count.empty()) {
		std::cerr << "ERROR: The page_count must not be empty!" << std::endl;
		return;
	}

	if (strtol(page_count.c_str(), nullptr, 10) == 0) {
		std::cerr << "ERROR: The page_count must be a positive integer!" << std::endl;
		return;
	}

	nlohmann::json book;
	book["title"] = title;
	book["author"] = author;
	book["genre"] = genre;
	book["publisher"] = publisher;
	book["page_count"] = strtol(page_count.c_str(), nullptr, 10);

	Request::method = "POST";
	Request::route = "/api/v1/tema/library/books";
	Request::body = book;

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
		std::cerr << "ERROR: Something went wrong adding the book!" << std::endl;
		return;
	}

	std::cout << "SUCCESS: Book added successfully!" << std::endl;
}
