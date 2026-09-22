#include "listener.h"
#include "cliopts.h"
#include "log.h"

#include <bits/pthreadtypes.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sched.h>

#include "packet.h"

#ifndef THREAD_COUNT
#define THREAD_COUNT 2
#endif


static int drp_bind(const char *ifname, in_addr_t address, uint16_t port) {
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

  char addrRepr[INET_ADDRSTRLEN*2 + 1] = {'\0'};
  inet_ntop(AF_INET, &address, addrRepr, sizeof(addrRepr)-1);
  LOG_PRINT("Bound to interface %s(%s:%u)\n", ifname, addrRepr, port);

  return sockfd;
}


// TODO: Implement message handling
static void *drp_server_thread_work(void *data) {
  Packet *packet = (Packet *)data;
  
  free(packet);
  return nullptr;
}

//static void print_message(size_t bufferSize, uint8_t buffer[bufferSize]) {
//  unsigned int windowSize = bufferSize-1;
//  for (; windowSize > 0; windowSize--) {
//    if (buffer[windowSize] != 0) {
//      break;
//    }
//  }
//  printf("Window size: %u\n", windowSize);
//  for (size_t i = 0; i <= windowSize; i++) {
//    if (i % 8 == 0 && i != 0) {
//      printf("\n");
//    }
//    printf("%x ", buffer[i]);
//  }
//  printf("\n");
//}

int drp_server_start(const char *ifname, in_addr_t address, uint16_t port) {
  int sockfd = drp_bind(ifname, address, port);
  if (sockfd < 0) {
    return sockfd;
  }
  
  pthread_t threads[1];

  uint8_t buffer[512] = {'\0'};
  struct sockaddr_in addr;
  socklen_t addrSize = sizeof(addr);

  LOG_PRINT("Server started\n");

  Packet *packet = nullptr;
  bool run = true;
  size_t received = 0;
  size_t i = 0;
  bool noMsg = false;
  //const atomic_bool trueVal = true;
  
  while (run) {
    received = recvfrom(sockfd, buffer, sizeof(buffer)/sizeof(buffer[0]), MSG_DONTWAIT, &addr, &addrSize);
    if (received == (size_t)-1) {
      noMsg = true;
      if (!noMsg) {
        LOG_PRINT("No message\n");
      }
      continue;
    }
    noMsg = false;

    packet = Packet_copy_message(buffer, received);
    pthread_create(&threads[0], nullptr, drp_server_thread_work, packet);
    LOG_PRINT("Started thread %zu work\n", i);
  }
  

  return sockfd;
}
