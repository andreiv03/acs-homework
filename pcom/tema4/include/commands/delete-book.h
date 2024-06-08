#ifndef DELETE_BOOK_H
#define DELETE_BOOK_H

#include "../client.h"
#include "../command.h"
#include "../request.h"
#include "../utils.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

class DeleteBookCommand : public Command {
public:
	void execute() const override;
};

#endif
