#include "include/bitstream.h"
#include <transmit.h>
#include <avr/io.h>
#include <radio.h>
#include <bitstream.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <message.h>
#include <util/crc16.h>
#include <util/delay.h>

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

typedef enum TransmitState {
    TRANSMIT_IDLE,
    TRANSMIT_PREAMBLE,
    TRANSMIT_MSG_TYPE,
    TRANSMIT_PAYLOAD_LEN,
    TRANSMIT_CRC,
    TRANSMIT_PAYLOAD,
    // send final "low <wait> high <wait>" sequence to tell the receiver we're done
    TRANSMIT_FINALIZING
} TransmitState;

typedef enum BitSentState {
  // set pin = low, wait RADIO_T_NANOS
  BITSTATE_LOW,
  // set pin = high, wait RADIO_T_NANOS
  BITSTATE_HIGH,
  BITSTATE_DONE
} BitSentState;
static volatile message nextMessage;

// IRQ handler data
static volatile TransmitState transmitState;
static volatile BitSentState bitState;

static uint8_t preamble[RADIO_PREAMBLE_SIZE_IN_BYTES];
static bitstream preambleBitstream;
static bitstream msgTypeBitstream;
static bitstream payloadLenBitstream;
static bitstream crcBitstream;
static bitstream payloadBitstream;

// invoked roughly every RADIO_T_NANOS (actual interval depends on CPU frequency and sysclk divider used)
ISR(TIMER0_COMPA_vect)
{
    // check whether we're currently sending a low-high or high-low sequence
    switch(bitState) {
        case BITSTATE_LOW:
            dataOutLow();
            bitState = BITSTATE_DONE;
            return;
        case BITSTATE_HIGH:
            dataOutHigh();
            bitState = BITSTATE_DONE;
            return;
        case BITSTATE_DONE:
            break;
    }

    // we're done sending a bit (or have not started yet)
    bitstream *bitstream;

try_again:
    switch(transmitState) {
        case TRANSMIT_IDLE:
            return;
        case TRANSMIT_PREAMBLE:
            bitstream = &preambleBitstream;
            break;
        case TRANSMIT_MSG_TYPE:
            bitstream = &msgTypeBitstream;
            break;
        case TRANSMIT_PAYLOAD_LEN:
            bitstream = &payloadLenBitstream;
            break;
        case TRANSMIT_CRC:
            bitstream = &crcBitstream;
            break;
        case TRANSMIT_PAYLOAD:
            bitstream = &payloadBitstream;
            break;
        case TRANSMIT_FINALIZING:
            transmitState = TRANSMIT_IDLE;
            return;
    }

    if ( bitstream_has_more_bits(bitstream) )
    {
        if ( bitstream_read_bit(bitstream) ) {
            // 4. If the bit equals “1”, then
            // 4.1. Set the output signal low
            // 4.2. Wait for mid-bit time (T)
            // 4.3. Set the output signal high
            // 4.4. Wait for mid-bit time (T)
            dataOutLow();
            bitState = BITSTATE_HIGH;
        } else {
            // 5. If the bit equals “0”, then
            // 5.1. Set the output signal high
            // 5.2. Wait for mid-bit time (T)
            // 5.3. Set the output signal low
            // 5.4. Wait for mid-bit time (T)
            dataOutHigh();
            bitState = BITSTATE_LOW;
        }
        return;
    }

    // the current bitstream ran out of bits,
    // advance to next state
    switch(transmitState) {
        case TRANSMIT_IDLE:
            return;
        case TRANSMIT_PREAMBLE:
            transmitState = TRANSMIT_MSG_TYPE;
            goto try_again;
        case TRANSMIT_MSG_TYPE:
            transmitState = TRANSMIT_PAYLOAD_LEN;
            goto try_again;
        case TRANSMIT_PAYLOAD_LEN:
            transmitState = TRANSMIT_CRC;
            goto try_again;
        case TRANSMIT_CRC:
            if ( nextMessage.payload_len != 0 ) {
                transmitState = TRANSMIT_PAYLOAD;
                goto try_again;
            }
            // $$FALL-THROUGH$$
        case TRANSMIT_PAYLOAD:
            transmitState = TRANSMIT_FINALIZING;
            bitState = BITSTATE_HIGH;
            dataOutLow();
            break;
        case TRANSMIT_FINALIZING:
            transmitState = TRANSMIT_IDLE;
            break;
    }
}

void transmit_init() {

    transmitState = TRANSMIT_IDLE;
    bitstream_for_reading( &preambleBitstream, preamble, sizeof preamble);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"

    bitstream_for_reading( &msgTypeBitstream, &nextMessage.msgType, sizeof nextMessage.msgType);
    bitstream_for_reading( &payloadLenBitstream, &nextMessage.payload_len, sizeof nextMessage.payload_len);
    bitstream_for_reading( &crcBitstream, &nextMessage.crc, sizeof nextMessage.crc);

#pragma GCC diagnostic pop

    DATA_OUT_DDR |= (1<<DATA_OUT_PIN);
    transmit_transmitter_state(false);
    SENDER_ONOFF_DDR |= (1<<SENDER_ONOFF_PIN);
}

void transmit_send_packet(uint8_t msgType, uint8_t payload_len, uint8_t *payload)
{
    while( transmitState != TRANSMIT_IDLE) {};

    nextMessage.msgType = msgType;
    nextMessage.payload_len = payload_len;

    // calculate crc
    uint8_t crc = 0x00;
    crc = _crc8_ccitt_update(crc, msgType );
    crc = _crc8_ccitt_update(crc, payload_len);
    if ( payload_len > 0 )
    {
      uint8_t *srcPtr=payload;
      volatile uint8_t *dstPtr=&nextMessage.payload[0];
      for ( uint8_t i = payload_len ; i > 0 ; i--) {
        uint8_t value = *srcPtr++;
        crc = _crc8_ccitt_update(crc, value);
        *dstPtr++ = value;
      }
    }
    nextMessage.crc = crc;

    bitstream_rewind( &preambleBitstream );
    bitstream_rewind( &msgTypeBitstream );
    bitstream_rewind( &payloadLenBitstream );
    bitstream_rewind( &crcBitstream );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
    bitstream_for_reading( &payloadBitstream, &nextMessage.payload[0], payload_len);
#pragma GCC diagnostic pop

    transmitState = TRANSMIT_PREAMBLE;
    bitState = BITSTATE_DONE;

    // setup TIMER0 interrupt
    cli();
    TCCR0B = 0; // stop Timer0
    TIMSK0 = 0; // disable Timer0 interrupts
    sei();

    TCNT0 = 0; // set timer start value
    OCR0A = 0x00; // set timer end value
    TCCR0A = (1<<WGM01); // enable CTC (clear timer on compare match) mode, timer loops at OCF0A
    TIMSK0 = (1<<OCIE0A); // enable Timer0 compare IRQ

    TCCR0B = RADIO_TIMER0_SYSCLK_DIV_BITS; // start Timer0 by setting frequency to sysclk/64
}

// switch actual transmitter on or off (useful for saving power, avoiding interference with receiver close to it)
void transmit_transmitter_state(bool onOff) {
    if ( onOff ) {
      senderOn();
      // wait some time for the transmitter to settle before we start sending data
      _delay_ms(100);
    } else {
      senderOff();
    }
}
