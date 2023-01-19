#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#define true 1
#define false 0

int analyse_traffic(char *data);
char *extract_network(char *ip);

void solve_task_two(FILE *input_file, FILE *output_file)
{
	char *buffer = NULL;
	size_t buffer_size = 0;

	getline(&buffer, &buffer_size, input_file);

	free(buffer);
	buffer = NULL;
	buffer_size = 0;

	while (true) {
		if (getline(&buffer, &buffer_size, input_file) == -1)
			break;

		buffer[strcspn(buffer, "\n")] = '\0';

		int verdict = analyse_traffic(buffer);
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
}

int analyse_traffic(char *traffic)
{
	char *origin_ip = NULL;
	char *response_ip = NULL;
	char *flow_duration = NULL;
	char *flow_packets_payload = NULL;

	int counter = 0;
	char *field = strtok(traffic, ",");

	while (field) {
		if (counter == 0) {
			origin_ip = calloc(strlen(field) + 1, sizeof(char));
			strcpy(origin_ip, field);
		}

		if (counter == 2) {
			response_ip = calloc(strlen(field) + 1, sizeof(char));
			strcpy(response_ip, field);
		}

		if (counter == 4) {
			flow_duration = calloc(strlen(field) + 1, sizeof(char));
			strcpy(flow_duration, field);
		}

		if (counter == 16) {
			flow_packets_payload = calloc(strlen(field) + 1, sizeof(char));
			strcpy(flow_packets_payload, field);
		}

		counter = counter + 1;
		field = strtok(NULL, ",");
	}

	float seconds = atof(strrchr(flow_duration, ':') + 1);
	float payload = atof(flow_packets_payload);
	free_allocated_memory(2, flow_duration, flow_packets_payload);

	char *origin_ip_network = extract_network(origin_ip);
	char *response_ip_network = extract_network(response_ip);
	free_allocated_memory(2, origin_ip, response_ip);

	if (seconds > 1) {
		if (payload == 0) {
			free_allocated_memory(2, origin_ip_network, response_ip_network);
			return false;
		}

		free_allocated_memory(2, origin_ip_network, response_ip_network);
		return true;
	}

	// if (strcmp(origin_ip_network, response_ip_network) == 0) {
	//   free_allocated_memory(2, origin_ip_network, response_ip_network);
	//   return true;
	// }

	free_allocated_memory(2, origin_ip_network, response_ip_network);

	return false;
}

char *extract_network(char *ip)
{
	size_t length = 0;

	if (strlen(ip) <= 15) {
		length = strcspn(ip, ".") + 1;
		if (strchr(ip, '.'))
			length = length + strcspn(strchr(ip, '.') + 1, ".");
	}

	char *network = calloc(length + 1, sizeof(char));
	strncpy(network, ip, length);
	return network;
}
