#ifndef UTILS
#define UTILS

#include <stdlib.h>

int max(int a, int b);
int min(int a, int b);

int ends_with(const char *string, const char *search);
char **split(char *string, char *separators, size_t *length);

void free_allocated_memory(size_t number, ...);
void free_allocated_memory_array(char **array, size_t length);

#endif
