#include <transmit.h>
#include <avr/io.h>
#include <radio.h>
#include <bitstream.h>
#include <stdint.h>

// TX DATA_OUT connected to D2
#define DATA_OUT_PIN 2
#define DATA_OUT_DDR DDRD
#define DATA_OUT_REG PORTD

#define SENDER_ONOFF_PIN 3
#define SENDER_ONOFF_DDR DDRD
#define SENDER_ONOFF_REG PORTD

#define dataOutHigh() DATA_OUT_REG|=(1<<DATA_OUT_PIN)
#define dataOutLow() DATA_OUT_REG&=~(1<<DATA_OUT_PIN)

#define senderOn() DATA_OUT_REG|=(1<<DATA_OUT_PIN)
#define senderOff() DATA_OUT_REG&=~(1<<DATA_OUT_PIN)

// switch actual transmitter on or off (useful for saving power, avoiding interference with receiver close to it)
void transmit_transmitter_state(bool onOff) {
    if ( onOff ) {
      senderOn();
    } else {
      senderOff();
    }
}

void transmit_init() {
    DATA_OUT_DDR |= (1<<DATA_OUT_PIN);
    transmit_transmitter_state(false);
    SENDER_ONOFF_DDR |= (1<<SENDER_ONOFF_PIN);
}

static void transmit_delay() {
    // FIXME: Delay for RADIO_T_NANOS
}

void send_zero()
{
    // 5. Else call ManchesterZero(T)
    // 5.1. Set the output signal high
    dataOutHigh();
    // 5.2. Wait for mid-bit time (T)
    transmit_delay();
    // 5.3. Set the output signal low
    dataOutLow();
    // 5.4. Wait for mid-bit time (T)
    transmit_delay();
}

void send_one()
{
    // 4. If the bit equals “1”, then call ManchesterOne(T)
    // 4.1. Set the output signal low
    dataOutLow();
    // 4.2. Wait for mid-bit time (T)
    transmit_delay();
    // 4.3. Set the output signal high
    dataOutHigh();
    // 4.4. Wait for mid-bit time (T)
    transmit_delay();
}

void transmit(bitstream *stream)
{
    // 1. Begin with the output signal high.
    dataOutHigh();
    // 2. Check if all bits have been sent, If yes, then go to step 7
    uint8_t bit = 0;
    while ( bitstream_has_more_bits(stream) )
    {
        // 3. Check the next logical bit to be coded
        bit = bitstream_read_bit(stream);
        if ( bit ) {
            send_one();
        } else {
            send_zero();
        }
    }
    // 7. Set output signal high and return
    send_one();
}
