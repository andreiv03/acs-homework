#ifndef UTILS
#define UTILS

#include <stdlib.h>

int ends_with(const char *string, const char *search);
char **split(char *string, char *separators, size_t *length);

void free_allocated_memory(size_t number, ...);
void free_allocated_memory_array(char **array, size_t length);

int min(int a, int b, int c);
int levenshtein_distance(char *string_one, char *string_two);

#endif
