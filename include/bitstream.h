#ifndef BITSTREAM_H
#define BITSTREAM_H
#include <stdint.h>
#include <stdbool.h>

/*
 * A stream of bits.
 */

// size must not exceed 255 bytes
#define BITSTREAM_BUFFER_SIZE_BYTES 128

typedef struct bitstream {
    uint8_t buffer[ BITSTREAM_BUFFER_SIZE_BYTES ];
    uint16_t bitsAvailable;
    uint8_t currentByteIdx;
    uint8_t currentMask;
} bitstream;

// setup bitstream for reading given len bytes of data from it
void bitstream_for_reading(bitstream *stream, uint8_t *data, uint8_t len);

// setup bitstream for writing to it
void bitstream_for_writing(bitstream *stream);

// clear a bitstream that was setup for writing
void bitstream_clear(bitstream *stream);

// returns the total size of this bitstream in bytes
// (bitstreams set up for writing will return the number of complete octets, disregarding partial transmission)
uint8_t bitstream_size_in_bytes(bitstream *stream);

// rewind bitstream to the beginning
void bitstream_rewind(bitstream *stream);

// check whether more bits can be read or written
bool bitstream_has_more_bits(bitstream *stream);

// write one bit. All values other than zero are considered to be a one-bit
void bitstream_write_bit(bitstream *stream, uint8_t bit);

// read a single bit. Return value is either zero or one.
uint8_t bitstream_read_bit(bitstream *stream);
#endif
