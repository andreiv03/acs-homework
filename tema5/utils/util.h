#ifndef UTIL_H_
#define UTIL_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define ERR(call_description)        \
  do {            \
    fprintf(stderr, "(%s, %d): ",    \
      __FILE__, __LINE__);    \
    perror(call_description);    \
  } while (0)

#define DIE(assertion, call_description)    \
  do {            \
    if (assertion) {      \
      ERR(call_description);    \
      exit(EXIT_FAILURE);    \
    }          \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif
