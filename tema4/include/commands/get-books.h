#ifndef GET_BOOKS_H
#define GET_BOOKS_H

#include "../client.h"
#include "../command.h"
#include "../request.h"
#include "../utils.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

class GetBooksCommand : public Command {
public:
	void execute() const override;
};

#endif
