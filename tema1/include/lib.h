#ifndef _SKEL_H_
#define _SKEL_H_

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PACKET_LEN 1600
#define ROUTER_NUM_INTERFACES 3

int send_to_link(int interface, char *frame_data, size_t length);

int recv_from_any_link(char *frame_data, size_t *length);

struct route_table_entry {
	uint32_t prefix;
	uint32_t next_hop;
	uint32_t mask;
	int interface;
} __attribute__((packed));

struct route_table {
	struct route_table_entry *entries;
	size_t length;
};

struct arp_table_entry {
	uint32_t ip;
	uint8_t mac[6];
};

struct arp_table {
	struct arp_table_entry *entries;
	size_t length;
};

char *get_interface_ip(int interface);

void get_interface_mac(int interface, uint8_t *mac);

uint16_t checksum(uint16_t *data, size_t length);

int hwaddr_aton(const char *txt, uint8_t *addr);

int read_rtable(const char *path, struct route_table_entry *rtable);

int parse_arp_table(char *path, struct arp_table_entry *arp_table);

void init(int argc, char *argv[]);

#define DIE(condition, message, ...) \
  do { \
    if ((condition)) { \
      fprintf(stderr, "[(%s:%d)]: " # message "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
      perror(""); \
      exit(1); \
    } \
  } while (0)

#endif
