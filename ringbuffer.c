#include <ringbuffer.h>

#include <error.h>

void ringbuffer_init(ringbuffer *buffer,uint8_t *storage, uint8_t storage_size) {
    buffer->data = storage;
    buffer->size = storage_size;
    buffer->read_ptr = 0;
    buffer->write_ptr = 0;
}

void ringbuffer_reset(ringbuffer *buffer) {
    buffer->read_ptr = 0;
    buffer->write_ptr = 0;
}

bool ringbuffer_is_full(ringbuffer *buffer) {
    return (buffer->write_ptr+1) % buffer->size == buffer->read_ptr;
}

bool ringbuffer_is_empty(ringbuffer *buffer) {
    return buffer->write_ptr == buffer->read_ptr;
}

void ringbuffer_write(ringbuffer *buffer, uint8_t data) {

#ifdef DEBUG
    if ( ringbuffer_is_full(buffer) ) {
      fail(ERROR_RINGBUFFER_OVERFLOW);
    }
#endif
    buffer->data[buffer->write_ptr] = data;
    buffer->write_ptr = (buffer->write_ptr+1) % buffer->size;
}

uint8_t ringbuffer_size(ringbuffer *buffer) {
    return buffer->size;
}

uint8_t ringbuffer_remaining_space(ringbuffer *buffer) {

    if ( buffer ->write_ptr >= buffer->read_ptr ) {
      return  buffer->size - ( buffer ->write_ptr - buffer->read_ptr ) - 1;
    }
    // write_ptr has wrapped already
    return buffer->size - ( buffer->read_ptr - buffer->write_ptr ) - 1;
}

// returns the number of actual bytes written
uint8_t ringbuffer_bulk_write(ringbuffer *buffer, uint8_t *data,uint8_t len) {
    uint8_t available = ringbuffer_remaining_space(buffer);
    uint8_t bytes_written = available >= len ? len : available;
    uint8_t bytesToCopy = bytes_written;
    uint8_t ptr = buffer->write_ptr;
    while ( bytesToCopy-- > 0 ) {
      buffer->data[ptr] = *data++;
      ptr = (ptr+1) % buffer->size;
    }
    buffer->write_ptr = ptr;
    return bytes_written;
}

uint8_t ringbuffer_read(ringbuffer *buffer)
{
    #ifdef DEBUG
    if ( ringbuffer_is_empty(buffer) ) {
      fail(ERROR_RINGBUFFER_UNDERFLOW);
    }
#endif
    uint8_t result = buffer->data[buffer->read_ptr];
    buffer->read_ptr = (buffer->read_ptr+1) % buffer->size;
    return result;
}

uint8_t ringbuffer_available_bytes(ringbuffer *buffer) {
    return buffer->size - ringbuffer_remaining_space(buffer)-1;
}

uint8_t ringbuffer_bulk_read(ringbuffer *buffer, uint8_t *destination, uint8_t size) {
    uint8_t bytesAvailable = ringbuffer_available_bytes(buffer);
    uint8_t read = size >= bytesAvailable ? bytesAvailable : size;
    uint8_t toRead = read;
    uint8_t ptr = buffer->read_ptr;
    while( toRead-- > 0 ) {
        *destination++ = buffer->data[ptr];
        ptr = (ptr+1) % buffer->size;
    }
    buffer->read_ptr = ptr;
    return read;
}
