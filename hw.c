#include <hw.h>
#include <avr/interrupt.h>
#include <radio.h>

/* #######################################
 * ############# ATmega328p ##############
 * #######################################
 */
#if defined (__AVR_ATmega328P__)

// ************ transmit.c **********

static volatile Runnable transmit_irq_handler;

ISR(TIMER0_COMPA_vect)
{
  transmit_irq_handler();
}

// start 8-bit timer IRQ
void transmit_start_timer_irq(Runnable fun)
{
    // setup TIMER0 interrupt
    cli();
    TCCR0B = 0; // stop Timer0
    TIMSK0 = 0; // disable Timer0 interrupts
    TIFR0 &= (1<<OCF0A); // clear interrupt flags
    transmit_irq_handler = fun;
    sei();

    TCNT0 = 0; // set timer start value
    OCR0A = RADIO_T_TIMER_TICKS_TO_WAIT; // timer end value
    TCCR0A = (1<<WGM01); // enable CTC (clear timer on compare match) mode, timer loops at OCF0A
    TIMSK0 = (1<<OCIE0A); // enable Timer0 compare IRQ

    TCCR0B = RADIO_TIMER0_SYSCLK_DIV_BITS; // start Timer0 by setting frequency to sysclk/64
}

void transmit_stop_timer_irq() {
    cli();
    TCCR0B = 0; // start Timer0 by setting frequency to sysclk/64
    TIFR0 &= (1<<OCF0A); // clear interrupt flags
    sei();
}

/* #######################################
 * ############# UNSUPPORTED #############
 * #######################################
 */
#else
#error Do not know for which MCU this is being built / MCU is not supported yet.
#endif
