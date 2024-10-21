#include "include/bitstream.h"
#include <transmit.h>
#include <hw.h>
#include <radio.h>
#include <bitstream.h>
#include <stdint.h>
#include <message.h>
#include <util/crc16.h>
#include <util/delay.h>

typedef enum TransmitState {
    // nothing to do
    TRANSMIT_IDLE,
    // sending actual message
    TRANSMIT_MESSAGE,
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

static volatile wire_message currentMessage;
static volatile bitstream msgBitstream;

// IRQ handler data
static volatile TransmitState transmitState;
static volatile BitSentState bitState;

// invoked roughly every RADIO_T_NANOS (actual interval depends on CPU frequency and sysclk divider used)
static void transmit_irq_handler()
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
    switch(transmitState) {
        case TRANSMIT_IDLE:
            return;
        case TRANSMIT_MESSAGE:
            if ( bitstream_has_more_bits(&msgBitstream) )
            {
                if ( bitstream_read_bit(&msgBitstream) ) {
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
            // end transmission
            transmitState = TRANSMIT_FINALIZING;
            bitState = BITSTATE_HIGH;
            dataOutLow();
            break;
        case TRANSMIT_FINALIZING:
            transmitState = TRANSMIT_IDLE;
            // TODO: Switch off timer IRQ
            return;
    }
}

void transmit_init() {
    DATA_OUT_DDR |= (1<<DATA_OUT_PIN);
    transmit_transmitter_state(false);
    SENDER_ONOFF_DDR |= (1<<SENDER_ONOFF_PIN);

    // write preamble once so we don't have to do it every time
    // we're sending a packet
    uint8_t *ptr = (uint8_t*) &currentMessage;
    for (uint8_t i = 0 ; i < RADIO_PREAMBLE_TX_COUNT ; i++) {
      *ptr++ = RADIO_PREAMBLE1;
      *ptr++ = RADIO_PREAMBLE2;
    }
    *ptr++ = RADIO_START_BYTE;
}

void transmit_send_packet(uint8_t msgType, uint8_t payload_len, uint8_t *payload)
{
    while( transmitState != TRANSMIT_IDLE) {};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
    bitstream_for_reading( &msgBitstream, (uint8_t*) &currentMessage, WIRE_MSG_SIZE_WITHOUT_PAYLOAD + payload_len);
#pragma GCC diagnostic pop

    currentMessage.message.msgType = msgType;
    currentMessage.message.payload_len = payload_len;

    // calculate crc
    uint8_t crc = 0x00;
    crc = _crc8_ccitt_update(crc, msgType );
    crc = _crc8_ccitt_update(crc, payload_len);
    if ( payload_len > 0 )
    {
      uint8_t *srcPtr=payload;
      volatile uint8_t *dstPtr=&currentMessage.message.payload[0];
      for ( uint8_t i = payload_len ; i > 0 ; i--) {
        uint8_t value = *srcPtr++;
        crc = _crc8_ccitt_update(crc, value);
        *dstPtr++ = value;
      }
    }
    currentMessage.message.crc = crc;

    transmitState = TRANSMIT_MESSAGE;
    bitState = BITSTATE_DONE;

    transmit_start_timer_irq(&transmit_irq_handler);
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
