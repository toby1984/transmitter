#ifndef HW_H
#define HW_H

#include <utils.h>
#include <defaults.h>

#if defined (__AVR_ATmega328P__)
#include <hw_atmega328p.h>
#else
#error Do not know for which MCU this is being built / MCU is not supported yet.
#endif

#include <utils.h>

// ====== used by transmit.c =================
void transmit_start_timer_irq(Runnable irqHandler);
void transmit_stop_timer_irq();

// ===========================
#endif
