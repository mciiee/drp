#ifndef LISTENER_H
#define LISTENER_H

#include <netinet/in.h>
#ifndef DRP_DEFAULT_PORT 
#define DRP_DEFAULT_PORT 53
#endif


int drp_bind(const char *ifname, in_addr_t address, uint16_t port);
#endif
