#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#define true 1
#define false 0

typedef struct {
	char **content;
	size_t length;
} file_data;

void read_domains(file_data *domains);
void read_extensions(file_data *extensions);

int check_url(char *url, file_data domains, file_data extensions);
int search_for_suspicious_extensions(char *url, file_data extensions);

void solve_task_one(FILE *input_file, FILE *output_file)
{
	file_data domains;
	domains.content = NULL;
	domains.length = 0;
	read_domains(&domains);

	file_data extensions;
	extensions.content = NULL;
	extensions.length = 0;
	read_extensions(&extensions);

	char *buffer = NULL;
	size_t buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, input_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		int verdict = check_url(buffer, domains, extensions);
		fprintf(output_file, "%d\n", verdict);

		free(buffer);
		buffer = NULL;
		buffer_size = 0;

		if (feof(input_file))
			break;
	}

	free(buffer);
	buffer = NULL;
	buffer_size = 0;

	for (size_t index = 0; index < domains.length; ++index)
		free(domains.content[index]);
	free(domains.content);

	for (size_t index = 0; index < extensions.length; ++index)
		free(extensions.content[index]);
	free(extensions.content);
}

void read_domains(file_data *domains)
{
	FILE *domains_file = fopen("data/urls/domains_database", "r");

	char *buffer = NULL;
	size_t buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, domains_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		const size_t LENGTH = domains->length + 1;
		domains->content = realloc(domains->content, LENGTH * sizeof(char *));
		domains->content[domains->length] = calloc(buffer_size, sizeof(char));
		strcpy(domains->content[domains->length], buffer);
		domains->length = domains->length + 1;

		free(buffer);
		buffer = NULL;
		buffer_size = 0;

		if (feof(domains_file))
			break;
	}

	free(buffer);
	buffer = NULL;
	buffer_size = 0;

	fclose(domains_file);
}

void read_extensions(file_data *exts)
{
	FILE *extensions_file = fopen("src/utils/extensions.txt", "r");

	char *buffer = NULL;
	size_t buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, extensions_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		const size_t LENGTH = exts->length + 1;
		exts->content = realloc(exts->content, LENGTH * sizeof(char *));
		exts->content[exts->length] = calloc(buffer_size, sizeof(char));
		strcpy(exts->content[exts->length], buffer);
		exts->length = exts->length + 1;

		free(buffer);
		buffer = NULL;
		buffer_size = 0;

		if (feof(extensions_file))
			break;
	}

	free(buffer);
	buffer = NULL;
	buffer_size = 0;

	fclose(extensions_file);
}

int check_url(char *url, file_data domains, file_data extensions)
{
	if (search_for_suspicious_extensions(url, extensions))
		return true;

	char *domain = calloc(strcspn(url, "/") + 1, sizeof(char));
	strncpy(domain, url, strcspn(url, "/"));

	for (size_t index = 0; index < domains.length; ++index) {
		if (ends_with(domain, domains.content[index])) {
			free_allocated_memory(1, domain);
			return true;
		}
	}

	int domain_digits_number = 0;
	int domain_dots_number = 0;

	for (size_t index = 0; index < strlen(domain); ++index) {
		if (isdigit(domain[index]))
			domain_digits_number = domain_digits_number + 1;
		if (domain[index] == '.')
			domain_dots_number = domain_dots_number + 1;
	}

	if (domain_digits_number > 3) {
		if (domain_digits_number >= 0.1 * strlen(domain)) {
			free_allocated_memory(1, domain);
			return true;
		}
	}

	if (domain_dots_number > 3) {
		free_allocated_memory(1, domain);
		return true;
	}

	free_allocated_memory(1, domain);
	return false;
}

int search_for_suspicious_extensions(char *url, file_data extensions)
{
	for (size_t index = 0; index < extensions.length; ++index)
		if (ends_with(url, extensions.content[index]))
			return true;

	return false;
}
