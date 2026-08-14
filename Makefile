CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c23 -g -D_POSIX_C_SOURCE=202312L

SRC_DIR=src
BUILD_DIR=build

$(BUILD_DIR)/drp: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $^ -o $@
