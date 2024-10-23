#include <debug_led.h>

#include <hw.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/delay.h>

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

void debug_led_blink(uint8_t times) {
    while ( times-- > 0 ) {
        debugLedOn();
        _delay_ms(1000);
        debugLedOff();
    _delay_ms(1000);
    }
}
