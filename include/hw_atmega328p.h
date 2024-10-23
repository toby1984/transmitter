#ifndef HW_ATMEGA328P
#define HW_ATMEGA328P
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <utils.h>
#include <radio.h>

// ====== transmit.c ==============

// 8-bit timer stuff
#define RADIO_TIMER0_SYSCLK_DIV 64L
#define RADIO_TIMER0_SYSCLK_DIV_BITS (1<<CS01)|(1<<CS00)
#define RADIO_TIMER0_NANOS_PER_TICK ((1000L*RADIO_TIMER0_SYSCLK_DIV)/(F_CPU/1000000L))

// #pragma message "RADIO_TIMER0_NANOS_PER_TICK: " XSTR(RADIO_TIMER0_NANOS_PER_TICK)

#define RADIO_T_TIMER_TICKS_TO_WAIT (RADIO_T_NANOS / RADIO_TIMER0_NANOS_PER_TICK )
// #pragma message "RADIO_T_TIMER_TICKS_TO_WAIT: " XSTR(RADIO_T_TIMER_TICKS_TO_WAIT)

#if RADIO_T_TIMER_TICKS_TO_WAIT > 255 || RADIO_T_TIMER_TICKS_TO_WAIT < 1
#error sysclk_div RADIO_TIMER0_SYSCLK_DIV is too low, 8-bit timer cannot measure up to RADIO_T_NANOS
#endif

// PD2 is connected to 433 MHz transmitter DATA line
#define DATA_OUT_PIN 2
#define DATA_OUT_DDR DDRD
#define DATA_OUT_REG PORTD

// PD5 is connected to a MOSFET that controls power to the transmitter
#define SENDER_ONOFF_PIN 5
#define SENDER_ONOFF_DDR DDRD
#define SENDER_ONOFF_REG PORTD

#define dataOutHigh() DATA_OUT_REG|=(1<<DATA_OUT_PIN)
#define dataOutLow() DATA_OUT_REG&=~(1<<DATA_OUT_PIN)

#define senderOn() DATA_OUT_REG|=(1<<DATA_OUT_PIN)
#define senderOff() DATA_OUT_REG&=~(1<<DATA_OUT_PIN)

// ========= debug_led.c ==============

#define DEBUG_LED_PIN 5
#define DEBUG_LED_DDR DDRB
#define DEBUG_LED_REG PORTB

#define debugLedOn() DEBUG_LED_REG |= (1<<DEBUG_LED_PIN)
#define debugLedOff() DEBUG_LED_REG &= ~(1<<DEBUG_LED_PIN)
#define debugLedToggle() DEBUG_LED_REG ^= (1<<DEBUG_LED_PIN)

// ============ serial.c ===============

#define BAUD 115200
#define serial_hw_write_data_register(data) UDR0 = data
#define serial_hw_wait_data_register_empty() while ( !( UCSR0A & (1<<UDRE0)) )

// ===================================
#endif
