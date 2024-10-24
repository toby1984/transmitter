#include <hw.h>
#include <serial.h>
#include <stdint.h>
#include <stdbool.h>
#include <error.h>
#include <avr/interrupt.h>
#include <ringbuffer.h>
#include <debug_led.h>

#define NOT_SET 0xff

static volatile uint8_t send_buffer_data[SERIAL_SEND_BUFFER_SIZE];
static volatile ringbuffer send_buffer;

static void serial_tx_irq() {
    // "send buffer empty" IRQ got triggered
    if ( ringbuffer_is_empty(&send_buffer) ) {
      // turn off IRQ as it will be generated as long as the UART data register is empty
      // enqueueing a new chunk will turn on the IRQ anyway
      serial_hw_set_tx_irq_enabled(false);
      return;
    }
    serial_hw_write_data_register( ringbuffer_read(&send_buffer) );
}

void serial_init() {
    ringbuffer_init(&send_buffer, &send_buffer_data[0],sizeof(send_buffer_data));
    serial_hw_init(&serial_tx_irq);
    sei();
}

void serial_send_async(uint8_t *data, uint8_t len)
{
    while ( len > 0 ) {
        uint8_t written = ringbuffer_bulk_write( &send_buffer, data, len );
        data += written;
        len -= written;
        if ( written == 0 ) {
            serial_hw_set_tx_irq_enabled(true);
            while ( ringbuffer_is_full( &send_buffer ) );
        }
    }
    serial_hw_set_tx_irq_enabled(true);
}

void serial_send_sync(uint8_t *data, uint8_t len) {
    serial_hw_send_bytes(data, len );
}
