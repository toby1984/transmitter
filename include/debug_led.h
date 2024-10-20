#ifndef DEBUG_LED_H
#define DEBUG_LED_H

#define DEBUG_LED_PIN 5
#define DEBUG_LED_DDR DDRB
#define DEBUG_LED_REG PORTB

#define debugLedOn() DEBUG_LED_REG |= (1<<DEBUG_LED_PIN)
#define debugLedOff() DEBUG_LED_REG &= ~(1<<DEBUG_LED_PIN)
#define debugLedToggle() DEBUG_LED_REG ^= (1<<DEBUG_LED_PIN)

void debug_led_init();
void debug_led(uint8_t onOff);
#endif
