#ifndef LOGOUT_H
#define LOGOUT_H

#include "../client.h"
#include "../command.h"
#include "../request.h"
#include "../utils.h"

#include <iostream>
#include <string>

class LogoutCommand : public Command {
public:
	void execute() const override;
};

#endif
