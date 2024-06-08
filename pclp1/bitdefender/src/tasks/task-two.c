#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#define true 1
#define false 0

int analyse_traffic(char *data);

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
	size_t list_length = 0;
	char **list = split(traffic, ",", &list_length);

	const size_t ORIGIN_IP = 0;
	const size_t ORIGIN_PORT = 1;
	const size_t RESPONSE_IP = 2;
	const size_t RESPONSE_PORT = 3;
	const size_t FLOW_DURATION = 4;
	const size_t FWD_PKTS_TOT = 5;
	const size_t BWD_PKTS_TOT = 6;
	const size_t FWD_HEADER_SIZE_TOT = 7;
	const size_t BWD_HEADER_SIZE_TOT = 8;
	const size_t FLOW_FIN_FLAG_COUNT = 9;
	const size_t FLOW_SYN_FLAG_COUNT = 10;
	const size_t FLOW_ACK_FLAG_COUNT = 11;
	const size_t FWD_PKTS_PAYLOAD_AVG = 12;
	const size_t BWD_PKTS_PAYLOAD_AVG = 13;
	const size_t FWD_IAT_AVG = 14;
	const size_t BWD_IAT_AVG = 15;
	const size_t FLOW_PKTS_PAYLOAD_AVG = 16;

	size_t oip_net_length = 0;
	char **origin_ip_network = split(list[ORIGIN_IP], ".", &oip_net_length);

	size_t rip_net_length = 0;
	char **response_ip_network = split(list[RESPONSE_IP], ".", &rip_net_length);

	int is_same_network = 0;
	if (strcmp(origin_ip_network[0], response_ip_network[0]) == 0)
		if (strcmp(origin_ip_network[1], response_ip_network[1]) == 0)
			is_same_network = 1;

	free_allocated_memory_array(origin_ip_network, oip_net_length);
	free_allocated_memory_array(response_ip_network, rip_net_length);

	float flow_average_payload = atof(list[FLOW_PKTS_PAYLOAD_AVG]);
	float fwd_iat = atof(list[FWD_IAT_AVG]);

	float flow_seconds = atof(strrchr(list[FLOW_DURATION], ':') + 1);
	int is_flow_duration_low = (flow_seconds < 0.1) ? 1 : 0;

	float headers_total_size = atof(list[FWD_HEADER_SIZE_TOT]);
	int abnormal_headers_size = (headers_total_size > 1000) ? 1 : 0;

	if (is_same_network && is_flow_duration_low && flow_average_payload) {
		free_allocated_memory_array(list, list_length);
		return true;
	}

	if (is_same_network && flow_seconds && flow_average_payload && fwd_iat) {
		free_allocated_memory_array(list, list_length);
		return true;
	}

	if (flow_seconds > 1 && flow_average_payload && abnormal_headers_size) {
		free_allocated_memory_array(list, list_length);
		return true;
	}

	free_allocated_memory_array(list, list_length);
	return false;
}
