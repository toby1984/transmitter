#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERROR_TIMER_ID_OUT_OF_RANGE=1, // a timer ID not supported on the current hardware
    ERROR_TIMER_DIVIDER_OUT_OF_RANGE, // sysclk divider out of range
    ERROR_TIMER_ALREADY_INITIALZED
} errorcode;

// disable interrupts and enter an infinite loop that blinks the debug LED according to the given errorcode
void fail(errorcode errorCode);

#endif
