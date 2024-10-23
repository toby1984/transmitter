#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct ringbuffer {
  uint8_t *data;
  uint8_t size;
  uint8_t read_ptr;
  uint8_t write_ptr;
} ringbuffer;

void ringbuffer_init(ringbuffer *buffer,uint8_t *storage, uint8_t storage_size);

void ringbuffer_reset(ringbuffer *buffer);

bool ringbuffer_is_full(ringbuffer *buffer);

bool ringbuffer_is_empty(ringbuffer *buffer);

void ringbuffer_write(ringbuffer *buffer, uint8_t data);

uint8_t ringbuffer_size(ringbuffer *buffer);

uint8_t ringbuffer_remaining_space(ringbuffer *buffer);

// returns the number of actual bytes written
uint8_t ringbuffer_bulk_write(ringbuffer *buffer, uint8_t *data,uint8_t len);

uint8_t ringbuffer_read(ringbuffer *buffer);

#endif
