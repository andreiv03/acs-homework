#include "ipv4.h"
#include "lib.h"
#include "protocols.h"
#include "queue.h"
#include "utils.h"

int main(int argc, char *argv[]) {
	init(argc - 2, argv + 2);
	
	char network_data[MAX_PACKET_LEN];
	
	struct route_table route_table;
	struct arp_table arp_table;
	init_tables(argv[1], &route_table, &arp_table);
	
	queue packets_queue = queue_create();
	
	while (1) {
		size_t network_data_length = 0;
		int interface = recv_from_any_link(network_data, &network_data_length);
		handle_ipv4_packet(network_data, network_data_length, &route_table, &arp_table, interface, packets_queue);
	}
}
