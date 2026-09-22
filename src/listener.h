#ifndef LISTENER_H
#define LISTENER_H

#include <netinet/in.h>
#include <stdatomic.h>

#ifndef DRP_DEFAULT_PORT 
#define DRP_DEFAULT_PORT 53
#endif

#ifndef DRP_DEFAULT_BACKLOG_SIZE
#define DRP_DEFAULT_BACKLOG_SIZE 4096
#endif

#ifndef DRP_SERVER_DEFAULT_BUFFER_SIZE
#define DRP_SERVER_DEFAULT_BUFFER_SIZE 4096
#endif

constexpr size_t UDP_HEADER_SIZE = 4;
constexpr size_t DNS_MESSAGE_SIZE = 12;
constexpr size_t DNS_FULL_MESSAGE_SIZE = 512;

int drp_server_start(const char *ifname, in_addr_t address, uint16_t port);
#endif
