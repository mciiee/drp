#include "packet.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


Packet *Packet_copy_message(uint8_t *buffer, size_t bufferSize) {
  Packet *packet = malloc(sizeof(*buffer) * bufferSize);
  memcpy(packet, buffer, bufferSize);
  return packet;
}

void Packet_free(Packet *packet) {
  free(packet);
}
