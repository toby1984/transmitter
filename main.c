#include <avr/io.h>
#include "util/delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <debug_led.h>

// TX DATA_OUT connected to D2
#define DATA_OUT_PIN 2
#define DATA_OUT_DDR DDRD
#define DATA_OUT_REG PORTD

#define dataOutHigh() DATA_OUT_REG|=(1<<DATA_OUT_PIN)
#define dataOutLow() DATA_OUT_REG&=~(1<<DATA_OUT_PIN)

#define TX_BIT_DELAY_MS 250

void tx_init() {
  DATA_OUT_DDR |= (1<<DATA_OUT_PIN); // switch to output
  DATA_OUT_REG &= ~(1<<DATA_OUT_PIN);
}

void tx_send(uint8_t* ptr, uint8_t len) {
   while ( len-- > 0 ) {
       uint8_t value = *ptr++;
       for ( uint8_t mask = 0x80 ; mask != 0 ; mask >>= 1) {
           uint8_t bit = value & mask;
           if ( bit != 0 ) {
             dataOutHigh();
             debug_led(1);
           } else {
             dataOutLow();
             debug_led(0);
           }
           _delay_ms( TX_BIT_DELAY_MS );
       }
   }
}

void main(void) {
    const uint8_t data[] = {0xaa};

    debug_led_init();
    tx_init();
    while(true) {
        tx_send( &data[0], 1);
       // _delay_ms(1000);
    }
}
