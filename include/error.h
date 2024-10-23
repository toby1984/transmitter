#ifndef ERROR_H
#define ERROR_H

#define DEBUG

/*
 * Application error codes and means to tell the user about them.
 */

typedef enum {
    ERROR_TIMER_ID_OUT_OF_RANGE=1, // a timer ID not supported on the current hardware
    ERROR_TIMER_DIVIDER_OUT_OF_RANGE, // sysclk divider out of range
    ERROR_TIMER_ALREADY_INITIALZED,
    // bitstream.c
    ERROR_BITSTREAM_OVERFLOW,
    ERROR_BITSTREAM_UNDERFLOW,
    // ringbuffer.c
    ERROR_RINGBUFFER_UNDERFLOW,
    ERROR_RINGBUFFER_OVERFLOW
} errorcode;

// disable interrupts and enter an infinite loop that blinks the debug LED according to the given errorcode
void fail(errorcode errorCode);

#endif
