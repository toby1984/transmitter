#include <hw.h>
#include <avr/interrupt.h>
#include <radio.h>
#include <serial.h>
#include <util/setbaud.h>

/* #######################################
 * ############# ATmega328p ##############
 * #######################################
 */
#if defined (__AVR_ATmega328P__)

// ************ transmit.c **********

static volatile Runnable transmit_irq_handler;
static volatile Runnable serial_tx_buffer_irq_handler;

// timer0, OCR0A reached
ISR(TIMER0_COMPA_vect)
{
  transmit_irq_handler();
}

// serial - data register empty, ready to send next byte
ISR(USART_UDRE_vect)
{
  serial_tx_buffer_irq_handler();
}

void transmit_init_hw() {
    DATA_OUT_DDR |= (1<<DATA_OUT_PIN);
    SENDER_ONOFF_DDR |= (1<<SENDER_ONOFF_PIN);
    senderOff();
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

// ************ serial.c **********

void serial_hw_set_tx_irq_enabled(bool onOff) {
    if ( onOff ) {
     UCSR0B |= (1<<UDRIE0);
    } else {
      UCSR0B &= ~(1<<UDRIE0);
    }
}

void serial_hw_init(Runnable tx_buffer_irq_handler) {

  serial_tx_buffer_irq_handler = tx_buffer_irq_handler;
  // Set baud rate
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
  // Set frame format: 8N1
  UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
  // Enable transmitter
  UCSR0B = (1<<TXEN0);
}

void serial_hw_send_byte(uint8_t toSend)
{
    serial_hw_wait_data_register_empty();
    serial_hw_write_data_register(toSend);
}

void serial_hw_send_bytes(uint8_t *data,uint8_t len)
{
  while ( len-- > 0 )
  {
    serial_hw_wait_data_register_empty();
    serial_hw_write_data_register(*data++);
  }
}

/* #######################################
 * ############# UNSUPPORTED #############
 * #######################################
 */
#else
#error Do not know for which MCU this is being built / MCU is not supported yet.
#endif
