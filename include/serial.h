#ifndef SERIAL_H
#define SERIAL_H

#include <serial.h>
#include <stdint.h>

#define SERIAL_SEND_BUFFER_SIZE 3

void serial_init();
void serial_send_async(uint8_t *data, uint8_t len);
void serial_send_sync(uint8_t *data, uint8_t len);

#endif
