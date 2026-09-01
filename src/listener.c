#include "listener.h"
#include "cliopts.h"
#include "log.h"

#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <arpa/inet.h>

int drp_bind(const char *ifname, in_addr_t address, uint16_t port) {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    LOG_ERROR("Failed to create a socket: [%s]\n", strerror(errno));
    return -1;
  }
  
  int err = 0;
  err = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifname, IFNAMSIZ);
  if (err < 0) {
    LOG_ERROR("Failed to set socket options: [%s]\n", strerror(errno));
    return -1;
  }

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = address;
  if (port == 0) {
    port = DRP_DEFAULT_PORT;
  }
  addr.sin_port = htons(port);

  err = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if (err < 0) {
    LOG_ERROR("Failed to bind a socket: [%s]\n", strerror(errno));
    return -1;
  }

  return sockfd;
}
