#include "../include/client.h"

std::map<std::string, CommandName> Client::commands;
int Client::socket_fd;
std::string Client::token;
std::vector<std::string> Client::cookies;

void Client::start() {
	Client::initialize_commands();
	Client::socket_fd = Utils::open_connection();

	while (true) {
		std::string command;
		std::getline(std::cin, command);

		if (command == "exit") {
			close(Client::socket_fd);
			break;
		}

		switch (Client::commands[command]) {
			case REGISTER: {
				auto *register_command = new(std::nothrow) RegisterCommand();
				register_command->execute();
				break;
			}

			case LOGIN: {
				auto *login_command = new(std::nothrow) LoginCommand();
				login_command->execute();
				break;
			}

			case ENTER_LIBRARY: {
				auto *enter_library_command = new(std::nothrow) EnterLibraryCommand();
				enter_library_command->execute();
				break;
			}

			case GET_BOOKS: {
				auto *get_books_command = new(std::nothrow) GetBooksCommand();
				get_books_command->execute();
				break;
			}

			case GET_BOOK: {
				auto *get_book_command = new(std::nothrow) GetBookCommand();
				get_book_command->execute();
				break;
			}

			case ADD_BOOK: {
				auto *add_book_command = new(std::nothrow) AddBookCommand();
				add_book_command->execute();
				break;
			}

			case DELETE_BOOK: {
				auto *delete_book_command = new(std::nothrow) DeleteBookCommand();
				delete_book_command->execute();
				break;
			}

			case LOGOUT: {
				auto *logout_command = new(std::nothrow) LogoutCommand();
				logout_command->execute();
				break;
			}

			default: {
				std::cerr << "ERROR: The command is not valid!" << std::endl;
				break;
			}
		}
	}
}

void Client::initialize_commands() {
	Client::commands["register"] = REGISTER;
	Client::commands["login"] = LOGIN;
	Client::commands["enter_library"] = ENTER_LIBRARY;
	Client::commands["get_books"] = GET_BOOKS;
	Client::commands["get_book"] = GET_BOOK;
	Client::commands["add_book"] = ADD_BOOK;
	Client::commands["delete_book"] = DELETE_BOOK;
	Client::commands["logout"] = LOGOUT;
	Client::commands["exit"] = EXIT;
}

int main() {
	Client::start();
	return 0;
}
