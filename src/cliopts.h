#ifndef CLIOPTS_H
#define CLIOPTS_H

#include <stddef.h>
#include <stdint.h>
struct CliOptions {
  char *ifname;
  char *configFilePath;
  size_t threadNum;
  uint16_t port;
  bool help;
};
#endif
