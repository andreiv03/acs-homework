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
void read_extensions(file_data *exts);
void read_tlds(file_data *tlds);
void read_websites(file_data *websites);

int check_url(char *url,
			  file_data domains,
				file_data extensions,
				file_data tlds,
				file_data websites);

int search_for_suspicious_extensions(char *url, file_data extensions);
int search_for_suspicious_tlds(char *domain, file_data tlds);
int search_for_suspicious_websites(char *domain, file_data websites);
int search_for_suspicious_content(char *url);

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

	file_data tlds;
	tlds.content = NULL;
	tlds.length = 0;
	read_tlds(&tlds);

	file_data websites;
	websites.content = NULL;
	websites.length = 0;
	read_websites(&websites);

	char *buffer = NULL;
	size_t buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, input_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		int verdict = check_url(buffer, domains, extensions, tlds, websites);
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

	for (size_t index = 0; index < tlds.length; ++index)
		free(tlds.content[index]);
	free(tlds.content);

	for (size_t index = 0; index < websites.length; ++index)
		free(websites.content[index]);
	free(websites.content);
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

void read_tlds(file_data *tlds)
{
	FILE *tlds_file = fopen("src/utils/tlds.txt", "r");

	char *buffer = NULL;
	size_t buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, tlds_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		const size_t LENGTH = tlds->length + 1;
		tlds->content = realloc(tlds->content, LENGTH * sizeof(char *));
		tlds->content[tlds->length] = calloc(buffer_size, sizeof(char));
		strcpy(tlds->content[tlds->length], buffer);
		tlds->length = tlds->length + 1;

		free(buffer);
		buffer = NULL;
		buffer_size = 0;

		if (feof(tlds_file))
			break;
	}

	free(buffer);
	buffer = NULL;
	buffer_size = 0;

	fclose(tlds_file);
}

void read_websites(file_data *websites)
{
	FILE *websites_file = fopen("src/utils/websites.txt", "r");

	char *buffer = NULL;
	size_t buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, websites_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		const size_t LENGTH = websites->length + 1;
		websites->content = realloc(websites->content, LENGTH * sizeof(char *));
		websites->content[websites->length] = calloc(buffer_size, sizeof(char));
		strcpy(websites->content[websites->length], buffer);
		websites->length = websites->length + 1;

		free(buffer);
		buffer = NULL;
		buffer_size = 0;

		if (feof(websites_file))
			break;
	}

	free(buffer);
	buffer = NULL;
	buffer_size = 0;

	fclose(websites_file);
}

int check_url(char *url,
			  file_data domains,
				file_data extensions,
				file_data tlds,
				file_data websites)
{
	if (search_for_suspicious_extensions(url, extensions))
		return true;

	if (search_for_suspicious_content(url))
		return true;

	char *domain = calloc(strcspn(url, "/") + 1, sizeof(char));
	strncpy(domain, url, strcspn(url, "/"));

	size_t queries_length = 0;
	char **queries_list = NULL;

	if (strchr(url, '?')) {
		const size_t QUERIES_LENGTH = strcspn(url, "#");
		char *queries = calloc(QUERIES_LENGTH, sizeof(char));
		strncpy(queries, strchr(url, '?') + 1, QUERIES_LENGTH);
		queries_list = split(queries, "&", &queries_length);
		free_allocated_memory(1, queries);
	}

	if (queries_length > 3) {
		free_allocated_memory(1, domain);
		free_allocated_memory_array(queries_list, queries_length);
		return true;
	}

	free_allocated_memory_array(queries_list, queries_length);

	for (size_t index = 0; index < domains.length; ++index) {
		if (ends_with(domain, domains.content[index])) {
			free_allocated_memory(1, domain);
			return true;
		}
	}

	if (search_for_suspicious_websites(domain, websites)) {
		free_allocated_memory(1, domain);
		return true;
	}

	if (search_for_suspicious_tlds(domain, tlds)) {
		free_allocated_memory(1, domain);
		return true;
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

int search_for_suspicious_tlds(char *domain, file_data tlds)
{
	for (size_t index = 0; index < tlds.length; ++index)
		if (ends_with(domain, tlds.content[index]))
			return true;

	return false;
}

int search_for_suspicious_websites(char *domain, file_data websites)
{
	const int MAX_DISTANCE = 2;

	for (size_t index = 0; index < websites.length; ++index)
		if (!ends_with(domain, websites.content[index])) {
			int dist = levenshtein_distance(domain, websites.content[index]);
			if (dist <= MAX_DISTANCE)
				return true;
		}

	return false;
}

int search_for_suspicious_content(char *url)
{
	if (!strchr(url, '/'))
		return true;

	if (strchr(url, '~'))
		return true;

	if (strstr(url, "wp-admin") || strstr(url, "host"))
		return true;

	if (strstr(url, "/exe/") || strstr(url, "/raw/"))
		return true;

	if (strstr(url, "?environment") || strstr(url, "=download"))
		return true;

	return false;
}
