#include <error.h>

#include <debug_led.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void fail(errorcode errorCode) {
    cli();
    debug_led_init();
    debugLedOff();
    while( true ) {
        for ( uint8_t cnt = errorCode ; cnt > 0 ; cnt-- ) {
            _delay_ms(1000);
            debugLedOn();
            _delay_ms(1000);
            debugLedOff();
        }
        _delay_ms(3000);
    }
}
