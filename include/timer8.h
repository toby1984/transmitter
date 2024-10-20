#ifndef TIMER8_H
#define TIMER8_H

#include <stdint.h>

// setup 8-bit timer to run forwards using the given clock divier
void timer8_init(uint8_t timerNo);

// load 8-bit value into timer
void timer8_load(uint8_t timerNo, uint8_t value);

// set 8-bit timer start counting forward
void timer8_start(uint8_t timerNo, uint16_t clockDivider);

// stop 8-bit timer and return the current counter value
uint8_t timer8_stop(uint8_t timerNo);

// use the given timer to wait a given amount of ticks
void timer8_delay(uint8_t ticks);
#endif
