CC=clang
# CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c23 -g -D_POSIX_VERSION=202405L
CFLAGS=-Wall -Wextra -Wpedantic -std=c23 -g -D_GNU_SOURCE

SRC_DIR=src
BUILD_DIR=build

$(BUILD_DIR)/drp: $(BUILD_DIR)/main.o $(BUILD_DIR)/listener.o $(BUILD_DIR)/packet.o
	$(CC) $^ -o $@


$(BUILD_DIR)/packet.o: $(SRC_DIR)/packet.c $(SRC_DIR)/packet.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/cliopts.h
	$(CC) -c $(CFLAGS) $< -o $@


$(BUILD_DIR)/listener.o: $(SRC_DIR)/listener.c $(SRC_DIR)/cliopts.h
	$(CC) -c $(CFLAGS) $< -o $@
