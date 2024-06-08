#ifndef COMMAND_H
#define COMMAND_H

enum CommandName {
	EXIT,
	REGISTER,
	LOGIN,
	ENTER_LIBRARY,
	GET_BOOKS,
	GET_BOOK,
	ADD_BOOK,
	DELETE_BOOK,
	LOGOUT
};

class Command {
public:
	virtual void execute() const = 0;
};

#endif
