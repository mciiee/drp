#ifndef PACKET_H
#define PACKET_H

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Packet {
  uint8_t *msg;
  size_t size;
} Packet;


Packet *Packet_copy_message(uint8_t *buffer, size_t bufferSize);
#endif
