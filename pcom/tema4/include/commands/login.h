#ifndef LOGIN_H
#define LOGIN_H

#include "../client.h"
#include "../command.h"
#include "../request.h"
#include "../utils.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

class LoginCommand : public Command {
public:
	void execute() const override;
};

#endif
