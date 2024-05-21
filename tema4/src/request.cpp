#include "../include/request.h"

std::string Request::method;
std::string Request::route;
nlohmann::json Request::body;

std::string Request::compute() {
	std::string protocol = "HTTP/1.1";
	std::stringstream request;

	request << Request::method << " " << Request::route << " " << protocol << Utils::LINE_SEPARATOR;
	request << "Host: " << Utils::IP << Utils::LINE_SEPARATOR;

	if (!Client::token.empty())
		request << "Authorization: Bearer " << Client::token << Utils::LINE_SEPARATOR;

	if (!Request::body.empty()) {
		request << "Content-Type: application/json" << Utils::LINE_SEPARATOR;
		request << "Content-Length: " << std::to_string(Request::body.dump().size()) << Utils::LINE_SEPARATOR;
	}

	if (!Client::cookies.empty()) {
		request << "Cookie: ";

		for (size_t index = 0; index < Client::cookies.size(); ++index) {
			request << Client::cookies[index];
			if (index != Client::cookies.size() - 1)
				request << "; ";
		}

		request << Utils::LINE_SEPARATOR;
	}

	request << Utils::LINE_SEPARATOR;

	if (!Request::body.empty())
		request << Request::body.dump() << Utils::LINE_SEPARATOR;

	return request.str();
}
