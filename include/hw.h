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
void transmit_hw_init();
void transmit_hw_start_timer_irq(Runnable irqHandler);
// void transmit_hw_stop_timer_irq();

// ===== used by serial.c ===========

void serial_hw_init(Runnable tx_buffer_irq_handler);
void serial_hw_send_bytes(uint8_t *data,uint8_t len);
void serial_hw_send_byte(uint8_t toSend);
void serial_hw_set_tx_irq_enabled(bool onOff);

// ===========================
#endif
