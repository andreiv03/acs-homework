#ifndef UTILS
#define UTILS

#include <stdlib.h>

int max(int a, int b);
int min(int a, int b);
int ends_with(const char *string, const char *search);
void free_allocated_memory(size_t number, ...);

#endif
