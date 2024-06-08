#ifndef ENTER_LIBRARY_H
#define ENTER_LIBRARY_H

#include "../client.h"
#include "../command.h"
#include "../request.h"
#include "../utils.h"

#include <iostream>
#include <string>

class EnterLibraryCommand : public Command {
public:
	void execute() const override;
};

#endif
