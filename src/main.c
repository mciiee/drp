#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>


#include <getopt.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "log.h"
#include "cliopts.h"
#include "listener.h"

#define HELPER_DEFAULT_OPTION(option) " (default: " option ")"

#ifndef DEFAULT_CONFIG_PATH
#define DEFAULT_CONFIG_PATH "~/.config/drp/config.lua"
#endif

#ifndef DEFAULT_INTERFACE
#define DEFAULT_INTERFACE "lo"
#endif


#ifndef DEFAULT_PORT
#define DEFAULT_PORT "53"
#endif



constexpr char CLI_OPTIONS_TEMPLATE[] = {
  "Usage: %s [OPTIONS]" "\n"
  "Options: " "\n"
  "  " "-c <config file path>" HELPER_DEFAULT_OPTION(DEFAULT_CONFIG_PATH) "\n" 
  "  " "-i <interface>" HELPER_DEFAULT_OPTION(DEFAULT_INTERFACE) "\n"
  "  " "-p <port>" HELPER_DEFAULT_OPTION(DEFAULT_PORT) "\n"
};

int CliOptions_init(struct CliOptions *cliOptions) {
  cliOptions->ifname = "lo";
  cliOptions->port = 53;
  cliOptions->help = false;
  cliOptions->threadNum = 2;
  
  char *username = getlogin();
  if (username == nullptr) {
    return -1;
  }
  
  struct passwd *pwd = getpwnam(username);
  if (pwd == nullptr) {
    return -2;
  }
  size_t userHomePathLength = strlen(pwd->pw_dir);
  size_t configFilePathSize = userHomePathLength + sizeof(DEFAULT_CONFIG_PATH);
  cliOptions->configFilePath = malloc(sizeof(char) * configFilePathSize);
  strncpy(cliOptions->configFilePath + userHomePathLength - 1, DEFAULT_CONFIG_PATH, configFilePathSize - userHomePathLength);
  strncpy(cliOptions->configFilePath, pwd->pw_dir, userHomePathLength);

  return 0;
}

void help_print(int argc, char * const argv[argc]) {
  printf(CLI_OPTIONS_TEMPLATE, argv[0]);
}

void CliOptions_parse(struct CliOptions *cliOptions, int argc, char *argv[argc]) {
  char c = '\0';
  
  while ((c = getopt(argc, argv, "+c:i:p:t:h")) != -1) {
    switch (c) {
      case 'c':
        cliOptions->configFilePath = optarg;
        break;
      case 'i':
        cliOptions->ifname = optarg;
        break;
      case 't':
        cliOptions->threadNum = (size_t)atoll(optarg);
        break;
      case 'p':
        cliOptions->port = (uint16_t)atoi(optarg);
        break;
      case 'h':
        cliOptions->help = true;
        break;
    }
  }
}


int main(int argc, char *argv[argc]) {
  int err = 0;
  struct CliOptions cliOptions = {0};
  err = CliOptions_init(&cliOptions);
  if (err < 0) {
    LOG_ERROR("Failed to initialize CLI options: \"%s\"\n", strerror(errno));
    return EXIT_FAILURE;
  }
 
  CliOptions_parse(&cliOptions, argc, argv);

  if (cliOptions.help) {
    help_print(argc, argv);
    return EXIT_SUCCESS;
  }

  int sockfd = drp_server_start("lo", INADDR_ANY, 5353, 1);
  if (sockfd < 0) {
    return EXIT_FAILURE;
  }

  close(sockfd);
  return EXIT_SUCCESS;
}
