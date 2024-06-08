#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "ipv4.h"
#include "lib.h"
#include "protocols.h"
#include "queue.h"
#include "utils.h"

#define MAX_ROUTE_TABLE_LENGTH 70000
#define MAX_ARP_TABLE_LENGTH 10

#define MIN(x, y) ((x) < (y) ? (x) : (y))

int comparator(const void *a, const void *b) {
	const struct route_table_entry *entry_one = (const struct route_table_entry *) a;
	const struct route_table_entry *entry_two = (const struct route_table_entry *) b;
	
	uint32_t difference = ntohl(entry_one->prefix) - ntohl(entry_two->prefix);
	if (difference != 0)
		return difference;
	
	return ntohl(entry_one->mask) - ntohl(entry_two->mask);
}

void init_tables(char *path, struct route_table *route_table, struct arp_table *arp_table) {
	route_table->entries = calloc(MAX_ROUTE_TABLE_LENGTH, sizeof(struct route_table_entry));
	route_table->length = read_rtable(path, route_table->entries);
	qsort(route_table->entries, route_table->length, sizeof(struct route_table_entry), comparator);
	
	arp_table->entries = calloc(MAX_ARP_TABLE_LENGTH, sizeof(struct arp_table_entry));
	arp_table->length = parse_arp_table("arp_table.txt", arp_table->entries);
}

struct route_table_entry *search_best_route_table_entry(struct route_table route_table, uint32_t ip_header_daddr,
                                                        int left, int right) {
	if (left > right)
		return NULL;
	
	int middle = (left + right) / 2;
	
	if (route_table.entries[middle].prefix == (ip_header_daddr & route_table.entries[middle].mask)) {
		struct route_table_entry *best_match = &route_table.entries[middle];
		return search_best_route_table_entry(route_table, ip_header_daddr, middle + 1, right) ?: best_match;
	}
	
	if (ntohl(route_table.entries[middle].prefix) < ntohl(ip_header_daddr))
		return search_best_route_table_entry(route_table, ip_header_daddr, middle + 1, right);
	
	return search_best_route_table_entry(route_table, ip_header_daddr, left, middle - 1);
}

uint8_t *search_mac_address_in_arp_table(struct arp_table *arp_table, uint32_t next_hop_ip) {
	for (int index = 0; index < arp_table->length; ++index)
		if (arp_table->entries[index].ip == next_hop_ip)
			return arp_table->entries[index].mac;
	
	return NULL;
}

void send_packet(char *network_data, size_t network_data_length, int interface, int code) {
	struct ether_header *network_data_ether_header = (struct ether_header *) network_data;
	struct iphdr *network_data_ip_header = (struct iphdr *) (network_data + sizeof(struct ether_header));
	struct icmphdr *network_data_icmp_header = (struct icmphdr *) (network_data + sizeof(struct ether_header) +
	                                                               sizeof(struct iphdr));
	
	if (code == 0 && network_data_icmp_header->type != 8)
		return;
	
	char packet[MAX_PACKET_LEN] = {0};
	size_t packet_length = MIN(network_data_length - sizeof(struct ether_header), sizeof(struct iphdr) + 64);
	if (code == 0)
		packet_length = network_data_length - sizeof(struct ether_header) - sizeof(struct iphdr) - sizeof(struct icmphdr);
	
	struct ether_header *packet_ether_header = (struct ether_header *) packet;
	struct iphdr *packet_ip_header = (struct iphdr *) (packet + sizeof(struct ether_header));
	struct icmphdr *packet_icmp_header = (struct icmphdr *) (packet + sizeof(struct ether_header) + sizeof(struct iphdr));
	
	packet_ether_header->ether_type = htons(IPv4_ETHER_TYPE);
	
	for (int index = 0; index < 6; ++index) {
		packet_ether_header->ether_dhost[index] = network_data_ether_header->ether_shost[index];
		packet_ether_header->ether_shost[index] = network_data_ether_header->ether_dhost[index];
	}
	
	packet_ip_header->saddr = htonl(inet_pton(AF_INET, get_interface_ip(interface), &packet_ip_header->saddr));
	packet_ip_header->daddr = network_data_ip_header->saddr;
	packet_ip_header->ihl = 5;
	packet_ip_header->version = 4;
	packet_ip_header->tos = 0;
	packet_ip_header->tot_len = htons(packet_length + sizeof(struct iphdr) + sizeof(struct icmphdr));
	packet_ip_header->id = htons(1);
	packet_ip_header->frag_off = 0;
	packet_ip_header->ttl = 64;
	packet_ip_header->protocol = 1;
	packet_ip_header->check = htons(checksum((uint16_t *) packet_ip_header, sizeof(struct iphdr)));
	
	packet_icmp_header->type = code;
	packet_icmp_header->code = 0;
	if (code == 0)
		packet_icmp_header->un.echo.id = network_data_icmp_header->un.echo.id;
	if (code == 0)
		packet_icmp_header->un.echo.sequence = network_data_icmp_header->un.echo.sequence;
	packet_icmp_header->checksum = htons(
			checksum((uint16_t *) packet_icmp_header, packet_length + sizeof(struct icmphdr)));
	
	memcpy(packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct icmphdr),
	       network_data + sizeof(struct ether_header) + sizeof(struct iphdr) + (code == 0 ? sizeof(struct icmphdr) : 0),
	       packet_length);
	
	send_to_link(interface, packet, ntohs(packet_ip_header->tot_len) + sizeof(struct ether_header));
}

void handle_mac_address_not_found(char *network_data, size_t network_data_length,
                                  struct route_table_entry *best_route_table_entry,
                                  struct arp_table *arp_table,
                                  queue packets_queue) {
	struct ether_header *ether_header = (struct ether_header *) network_data;
	ether_header->ether_type = htons(ARP_ETHER_TYPE);
	
	struct arp_header arp_header;
	arp_header.htype = htons(1);
	arp_header.ptype = htons(IPv4_ETHER_TYPE);
	arp_header.hlen = 6;
	arp_header.plen = 4;
	arp_header.op = htons(1);
	
	struct packet *packet = calloc(1, sizeof(struct packet));
	memcpy(packet->buffer, network_data, network_data_length);
	packet->length = network_data_length;
	
	queue_enq(packets_queue, packet);
	
	get_interface_mac(best_route_table_entry->interface, ether_header->ether_shost);
	get_interface_mac(best_route_table_entry->interface, arp_header.sha);
	arp_header.spa = inet_addr(get_interface_ip(best_route_table_entry->interface));
	arp_header.tpa = best_route_table_entry->next_hop;
	
	unsigned char broadcast_mac_address[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	memcpy(ether_header->ether_dhost, broadcast_mac_address, sizeof(ether_header->ether_dhost));
	memcpy(arp_header.tha, broadcast_mac_address, sizeof(broadcast_mac_address));
	
	memcpy(packet->buffer, &arp_header, sizeof(struct arp_header));
	packet->length = sizeof(struct ether_header) + sizeof(struct arp_header);
}