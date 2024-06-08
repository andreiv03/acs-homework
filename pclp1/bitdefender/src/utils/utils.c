#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

int ends_with(const char *string, const char *search)
{
	size_t string_length = strlen(string);
	size_t search_length = strlen(search);

	if (string_length < search_length)
		return false;

	for (size_t index = 0; index < search_length; ++index) {
		char character_one = string[string_length - search_length + index];
		char character_two = search[index];
		if (tolower(character_one) != tolower(character_two))
			return false;
	}

	return true;
}

char **split(char *string, char *separators, size_t *length)
{
	char **list = calloc(*length, sizeof(char *));
	char *item = strtok(string, separators);

	while (item) {
		list = realloc(list, (*length + 1) * sizeof(char *));
		list[*length] = calloc(strlen(item) + 1, sizeof(char));
		strcpy(list[*length], item);
		*length = *length + 1;

		item = strtok(NULL, separators);
	}

	return list;
}

void free_allocated_memory(size_t number, ...)
{
	va_list list;
	va_start(list, number);

	for (size_t index = 0; index < number; ++index)
		free(va_arg(list, char *));

	va_end(list);
}

void free_allocated_memory_array(char **array, size_t length)
{
	for (size_t index = 0; index < length; ++index)
		free(array[index]);
	free(array);
}

int min(int a, int b, int c)
{
	int aux = a;
	if (aux > b)
		aux = b;
	if (aux > c)
		aux = c;
	return aux;
}

int levenshtein_distance(char *string_one, char *string_two)
{
	int length_one = strlen(string_one);
	int length_two = strlen(string_two);
	int dp[length_one + 1][length_two + 1];

	for (size_t i = 0; i <= length_one; ++i)
		dp[i][0] = i;

	for (size_t j = 0; j <= length_two; ++j)
		dp[0][j] = j;

	for (size_t i = 1; i <= length_one; ++i) {
		for (size_t j = 1; j <= length_two; ++j) {
			int cost = (string_one[i - 1] == string_two[j - 1]) ? 0 : 1;
			dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1,
						   dp[i - 1][j - 1] + cost);
		}
	}

	return dp[length_one][length_two];
}
