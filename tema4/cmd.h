#ifndef _CMD_H
#define _CMD_H

#include "../util/parser/parser.h"

#define SHELL_EXIT -100

int parse_command(command_t *cmd, int level, command_t *father);

#endif
