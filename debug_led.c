#include <debug_led.h>

#include <avr/io.h>
#include <stdint.h>

void debug_led_init() {
    DEBUG_LED_DDR |= (1<<DEBUG_LED_PIN); // switch to output
    debugLedOff();
}

void debug_led_toggle() {
    debugLedToggle();
}

void debug_led(uint8_t onOff) {
    if ( onOff ) {
        debugLedOn();
    } else {
        debugLedOff();
    }
}
