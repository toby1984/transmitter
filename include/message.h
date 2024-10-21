#ifndef MESSAGE_H
#define MESSAGE_H

#include <radio.h>
#include <stdint.h>

/*
 * Message formats.
 */

// size of message header only (WITHOUT payload or preamble)
#define MESSAGE_HEADER_SIZE 3

#define MESSAGE_MAX_PAYLOAD_SIZE (RADIO_MTU - RADIO_PREAMBLE_SIZE_IN_BYTES - MESSAGE_HEADER_SIZE)

typedef struct message {
  // make sure to adjust MESSAGE_HEADER_SIZE when changing fields here
  uint8_t msgType;
  uint8_t payload_len;
  uint8_t crc;
  uint8_t payload[MESSAGE_MAX_PAYLOAD_SIZE];
} message;

#define MESSAGE_SIZE_WITHOUT_PAYLOAD sizeof(((message*) 0)->msgType) + sizeof(((message*) 0)->payload_len) + sizeof(((message*) 0)->crc)

// radio protocol wire format
typedef struct wire_message {
  uint8_t preamble[RADIO_PREAMBLE_SIZE_IN_BYTES];
  message message;
} wire_message;

#define WIRE_MSG_SIZE_WITHOUT_PAYLOAD MESSAGE_SIZE_WITHOUT_PAYLOAD + sizeof(((wire_message*) 0)->preamble)

#endif
