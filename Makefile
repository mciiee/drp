CC=clang
# CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c23 -g -D_POSIX_VERSION=202405L
CFLAGS=-Wall -Wextra -Wpedantic -std=c23 -g -D_GNU_SOURCE -DDEBUG

SRC_DIR=src
BUILD_DIR=build

LFLAGS=$(shell pkg-config --libs $(LUA_LIB))
IFLAGS=$(shell pkg-config --cflags $(LUA_LIB))

LUA_LIB=lua



$(BUILD_DIR)/drp: $(BUILD_DIR)/main.o $(BUILD_DIR)/listener.o $(BUILD_DIR)/packet.o $(BUILD_DIR)/config.o
	$(CC) $^ -o $@ $(LFLAGS)

$(BUILD_DIR)/config.o: $(SRC_DIR)/config.c $(SRC_DIR)/config.h
	$(CC) -c $(CFLAGS) $< -o $@ $(IFLAGS)

$(BUILD_DIR)/packet.o: $(SRC_DIR)/packet.c $(SRC_DIR)/packet.h
	$(CC) -c $(CFLAGS) $< -o $@ $(IFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/cliopts.h
	$(CC) -c $(CFLAGS) $< -o $@ $(IFLAGS)

$(BUILD_DIR)/listener.o: $(SRC_DIR)/listener.c $(SRC_DIR)/cliopts.h
	$(CC) -c $(CFLAGS) $< -o $@ $(IFLAGS)
