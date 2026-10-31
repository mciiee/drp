#ifndef CONFIG_H
#define CONFIG_H

#include <net/if.h>
#include <lua.h>
#include <netinet/in.h>
#include <stdint.h>

#ifndef IP_ADDRESS_STR_LENGTH
#define IP_ADDRESS_STR_LENGTH INET6_ADDRSTRLEN
#endif

enum DnsTransportProtocol: uint8_t {
  DNS_PROTO_UDP = 0,
  DNS_PROTO_TCP = 1
};

enum DnsIPVersion: uint8_t {
  DNS_PROTO_IPv4 = 0,
  DNS_PROTO_IPv6 = 1,
};


typedef struct Handler {
  char ifname[IFNAMSIZ];
  char ip[IP_ADDRESS_STR_LENGTH];
  uint16_t port;
  enum DnsTransportProtocol proto;
  enum DnsIPVersion ipv;
} Handler;



typedef struct DrpConfig {
  lua_State *state;
} DrpConfig;

const DrpConfig *drp_config_get(void);
int drp_config_load(const char *filename);
// const Handler *drp_config_get_handler(DrpConfig *config);

// Assumes [handler->ip] is allocated
// Return [nullptr] on error
const Handler *drp_config_lua_get_handler(const DrpConfig * config, Handler *const handler, const char *domain);
#endif
