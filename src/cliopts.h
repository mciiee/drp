#ifndef CLIOPTS_H
#define CLIOPTS_H
#include <stdint.h>
struct CliOptions {
  char *ifname;
  char *configFilePath;
  uint16_t port;
  bool help;
};
#endif
