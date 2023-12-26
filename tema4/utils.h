#ifndef _UTILS_H
#define _UTILS_H

#include "../util/parser/parser.h"

#define DIE(assertion, call_description)      \
  do {              \
    if (assertion) {        \
      fprintf(stderr, "(%s, %s, %d): ",  \
        __FILE__, __func__, __LINE__);  \
      perror(call_description);    \
      exit(EXIT_FAILURE);      \
    }            \
  } while (0)

char *get_word(word_t *s);

char **get_argv(simple_command_t *command, int *size);

#endif
