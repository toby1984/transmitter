#include <timer8.h>
#include <stdint.h>
#include <error.h>
#include <avr/io.h>

// setup 8-bit timer to run forwards using the given clock clockDivider
static uint8_t allocated[] = {0,0};

void timer8_init(uint8_t timerNo) {

    switch( timerNo ) {
      case 0: // COUNTER0 on ATMEGA328P
        if ( allocated[ timerNo ] != 0 ) {
          fail(ERROR_TIMER_ALREADY_INITIALZED);
        }
        allocated[timerNo] = 1;
        break;
      case 1: // COUNTER2 on ATMEGA328P
        if ( allocated[ timerNo ] != 0 ) {
          fail(ERROR_TIMER_ALREADY_INITIALZED);
        }
        allocated[timerNo] = 1;
        break;
      default:
        fail( ERROR_TIMER_ID_OUT_OF_RANGE ); // never returns
    }
}

// set 8-bit timer start counting forward
void timer8_start(uint8_t timerNo, uint16_t clockDivider) {

    uint8_t clockBits = 1;
    switch( clockDivider ) {
        case 1: clockBits = (1<<CS00); break;
        case 8: clockBits = (1<<CS01); break;
        case 64: clockBits = (1<<CS01)|(1<<CS00); break;
        case 256: clockBits = (1<<CS02); break;
        case 1024: clockBits = (1<<CS02)|(1<<CS00); break;
        default:
        fail( ERROR_TIMER_DIVIDER_OUT_OF_RANGE ); // never returns
    }
    switch( timerNo ) {
      case 0: // COUNTER0 on ATMEGA328P
        TCCR0B = clockBits;
        break;
      case 1: // COUNTER2 on ATMEGA328P
        TCCR2B = clockBits;
        break;
      default:
        fail( ERROR_TIMER_ID_OUT_OF_RANGE ); // never returns
    }
}

// stop 8-bit timer and return the current counter value
uint8_t timer8_stop(uint8_t timerNo)
{
    switch( timerNo ) {
      case 0: // COUNTER0 on ATMEGA328P
        TCCR0B &= ~0b111;
        return TCNT0;
      case 1: // COUNTER2 on ATMEGA328P
        TCCR2B &= ~0b111;
        return TCNT2;
      default:
        fail( ERROR_TIMER_ID_OUT_OF_RANGE ); // never returns
    }
    // never reached
    return 0xff;
}
