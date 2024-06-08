#ifndef _IPV4_H_
#define _IPV4_H_

#include "lib.h"
#include "queue.h"

#define ECHO_REPLY_TYPE 0
#define DESTINATION_UNREACHABLE_TYPE 3
#define TIME_EXCEDEED_TYPE 11

void handle_ipv4_packet(char *network_data, size_t network_data_length, struct route_table *route_table,
                        struct arp_table *arp_table, int interface, queue packets_queue);

#endif
