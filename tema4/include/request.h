#ifndef REQUEST_H
#define REQUEST_H

#include "client.h"
#include "utils.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

class Request {
public:
	static std::string method;
	static std::string route;
	static nlohmann::json body;

	static std::string compute();
};

#endif
