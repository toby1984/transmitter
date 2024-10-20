#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <stdint.h>
#include <stdbool.h>

// initialize
void transmit_init();

// switch actual transmitter on or off (useful for saving power, avoiding interference with receiver close to it)
void transmit_transmitter_state(bool onOff);

// transmit up to 255 bytes of data
void transmit_send_packet(uint8_t msgType, uint8_t payload_len, uint8_t *payload);
#endif
