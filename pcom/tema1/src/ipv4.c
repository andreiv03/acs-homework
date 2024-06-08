#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "ipv4.h"
#include "lib.h"
#include "protocols.h"
#include "queue.h"
#include "utils.h"

void handle_ipv4_packet(char *network_data, size_t network_data_length, struct route_table *route_table,
                        struct arp_table *arp_table, int interface, queue packets_queue) {
	struct ether_header *ether_header = (struct ether_header *) network_data;
	struct iphdr *ip_header = (struct iphdr *) (network_data + sizeof(struct ether_header));
	
	if (ntohs(ether_header->ether_type) != IPv4_ETHER_TYPE)
		return;
	
	uint32_t ip_checksum = checksum((uint16_t *) ip_header, sizeof(struct iphdr));
	if (ntohs(ip_checksum))
		return;
	
	if (ip_header->ttl <= 1) {
		send_packet(network_data, network_data_length, interface, TIME_EXCEDEED_TYPE);
		return;
	}
	
	if (ip_header->daddr == inet_addr(get_interface_ip(interface))) {
		send_packet(network_data, network_data_length, interface, ECHO_REPLY_TYPE);
		return;
	}
	
	struct route_table_entry *best_route_table_entry = search_best_route_table_entry(*route_table, ip_header->daddr, 0,
	                                                                                 route_table->length - 1);
	if (best_route_table_entry == NULL) {
		send_packet(network_data, network_data_length, interface, DESTINATION_UNREACHABLE_TYPE);
		return;
	}
	
	ip_header->ttl = ip_header->ttl - 1;
	ip_header->check = ~(~ip_header->check + ~((uint16_t)(ip_header->ttl + 1)) + (uint16_t)(ip_header->ttl)) - 1;
	
	uint8_t *mac_address = search_mac_address_in_arp_table(arp_table, best_route_table_entry->next_hop);
	if (mac_address == NULL)
		handle_mac_address_not_found(network_data, network_data_length, best_route_table_entry, arp_table, packets_queue);
	
	memcpy(ether_header->ether_dhost, mac_address, sizeof(ether_header->ether_dhost));
	get_interface_mac(best_route_table_entry->interface, ether_header->ether_shost);
	send_to_link(best_route_table_entry->interface, network_data, network_data_length);
}