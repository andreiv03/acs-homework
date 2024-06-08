#ifndef _UTILS_H_
#define _UTILS_H_

#include "lib.h"

#define ARP_ETHER_TYPE 0x0806
#define IPv4_ETHER_TYPE 0x0800

struct packet {
	int length;
	char buffer[MAX_PACKET_LEN];
} __attribute__((packed));

void init_tables(char *path, struct route_table *route_table, struct arp_table *arp_table);

struct route_table_entry *search_best_route_table_entry(struct route_table route_table, uint32_t ip_header_daddr,
                                                        int left, int right);

uint8_t *search_mac_address_in_arp_table(struct arp_table *arp_table, uint32_t next_hop_ip);

void send_packet(char *network_data, size_t network_data_length, int interface, int code);

void handle_mac_address_not_found(char *network_data, size_t network_data_length,
                                  struct route_table_entry *best_route_table_entry,
                                  struct arp_table *arp_table,
                                  queue packets_queue);

#endif
