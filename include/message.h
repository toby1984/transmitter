#ifndef MESSAGE_H
#define MESSAGE_H

#include <radio.h>
#include <stdint.h>

#define MESSAGE_MAX_PAYLOAD_SIZE (255-RADIO_PREAMBLE_SIZE_IN_BYTES-3)

typedef struct message {
  uint8_t msgType;
  uint8_t payload_len;
  uint8_t crc;
  uint8_t payload[MESSAGE_MAX_PAYLOAD_SIZE];
} message;
#endif
