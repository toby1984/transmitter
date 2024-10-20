#ifndef RADIO_H
#define RADIO_H

// max. bit rate supported by hardware.
// actual bitrate available to the application will be half of it because of Manchester encoding.
#define RADIO_BITS_PER_SECOND 9000

// Manchester encoding: T
#define RADIO_T_NANOS (1000000000/RADIO_BITS_PER_SECOND)
// Manchester encoding: 2T
#define RADIO_2T_NANOS (2*RADIO_T_NANOS)

// max. timing deviation during Manchester decoding
// (needs to be less than 0.5)
#define RADIO_TIMING_SLACK_PERCENTAGE 0.25

// Manchester encoding: min/max thresholds to detect T
#define RADIO_T_MIN_NANOS (RADIO_T_NANOS - RADIO_T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)
#define RADIO_T_MAX_NANOS (RADIO_T_NANOS + RADIO_T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)

// Manchester encoding: min/max thresholds to detect 2T
#define RADIO_2T_MIN_NANOS (RADIO_2T_NANOS - RADIO_2T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)
#define RADIO_2T_MAX_NANOS (RADIO_2T_NANOS + RADIO_2T_NANOS*RADIO_TIMING_SLACK_PERCENTAGE)

#endif
