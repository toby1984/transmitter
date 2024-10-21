#ifndef DEBUG_LED_H
#define DEBUG_LED_H

#include <avr/io.h>
#include <hw.h>

/*
 * Control debug LED.
 */
void debug_led_init();
void debug_led(uint8_t onOff);
#endif
