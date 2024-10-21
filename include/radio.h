#ifndef RADIO_H
#define RADIO_H

#include <defaults.h>
#include <stdint.h>
#include <avr/io.h>
#include <utils.h>

// Maximum size of a packet on the wire/air).
//
// My code is using uint8_t everywhere so we can loop over at most 255 bytes
// plus cheap MCUs are very limited on RAM anyway and sending larger packets over the air
// would get expensive pretty fast when having to repeat larger packets because of transmissione errors.
#define RADIO_MTU 255

// how many times the sequence <RADIO_PREAMBLE1><RADIO_PREAMBLE2> will be transmitted before sending
// RADIO_START_BYTE followed by the actual message
#define RADIO_PREAMBLE_TX_COUNT 3

// 2 preamble types * RADIO_PREAMBLE_TX_COUNT + 1 byte (RADIO_START_BYTE)
#define RADIO_PREAMBLE_SIZE_IN_BYTES ((2*RADIO_PREAMBLE_TX_COUNT)+1)

#define RADIO_PREAMBLE1 0x0b
#define RADIO_PREAMBLE2 0x77
#define RADIO_START_BYTE 0xab;

// max. bit rate supported by hardware / brutto bit rate.
// The actual (netto) bitrate available to the application will be half of it because of Manchester encoding.
#define RADIO_BITS_PER_SECOND 9000

// Manchester encoding: T
#define RADIO_T_NANOS (1000000000L/RADIO_BITS_PER_SECOND)

#pragma message "RADIO_T_NANOS: " XSTR(RADIO_T_NANOS)

// Manchester encoding: 2T
#define RADIO_2T_NANOS (2*RADIO_T_NANOS)

// max. timing deviation during Manchester decoding
// (needs to be less than 0.5)
#define RADIO_TIMING_SLACK_PERCENTAGE 0.25
#pragma message "RADIO_TIMING_SLACK_PERCENTAGE: " XSTR(RADIO_TIMING_SLACK_PERCENTAGE)

// Manchester encoding: min/max thresholds to detect T
#define RADIO_T_MIN_NANOS (RADIO_T_NANOS - RADIO_T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)
#define RADIO_T_MAX_NANOS (RADIO_T_NANOS + RADIO_T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)

// Manchester encoding: min/max thresholds to detect 2T
#define RADIO_2T_MIN_NANOS (RADIO_2T_NANOS - RADIO_2T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)
#define RADIO_2T_MAX_NANOS (RADIO_2T_NANOS + RADIO_2T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)

// write message preamble data to an array of at least RADIO_PREAMBLE_SIZE_IN_BYTES bytes in size
void radio_init_preamble(uint8_t *data);

#endif
