#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <stdint.h>
#include <stdbool.h>

// initialize
void transmit_init();

// switch actual transmitter on or off (useful for saving power, avoiding interference with receiver close to it)
void transmit_transmitter_state(bool onOff);

// transmit up to 255 bytes of data
void transmit(uint8_t *buffer, uint8_t len);
#endif
