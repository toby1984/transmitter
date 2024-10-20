#include <bitstream.h>

/*
typedef struct bitstream {
    uint8_t buffer[ BITSTREAM_BUFFER_SIZE_BYTES ];
    uint16_t bitsAvailable;
    uint8_t currentByteIdx = 0;
    uint8_t currentMask = 0x80;
    bool error;
} bitstream;
 */
void bitstream_for_reading(bitstream *stream, uint8_t *data, uint8_t len) {

    stream->bitsAvailable = len*8;
    stream->currentByteIdx = 0;
    stream->currentMask = 0x80;
    stream->error = false;

    uint8_t *ptr = &(stream->buffer[0]);
    while( len-- > 0 ) {
        *ptr++ = *data++;
    }
}

// setup bitstream for writing to it
void bitstream_for_writing(bitstream *stream) {
    stream->bitsAvailable = 0;
    stream->currentByteIdx = 0;
    stream->currentMask = 0x80;
    stream->error = false;
}

// rewind bitstream to the beginning
void bitstream_rewind(bitstream *stream) {
    stream->error = false;
    stream->currentByteIdx = 0;
    stream->currentMask = 0x80;
}

// clear a bitstream that was setup for writing
void bitstream_clear(bitstream *stream) {
    stream->bitsAvailable = 0;
    bitstream_rewind(stream);
}

// returns the total size of this bitstream in bytes
// (bitstreams set up for writing will return the number of complete octets, disregarding partial transmission)
uint8_t bitstream_size_in_bytes(bitstream *stream) {
    return stream->bitsAvailable / 8;
}

// check whether more bits can be read or written
bool bitstream_has_more_bits(bitstream *stream) {
    return stream->currentByteIdx < bitstream_size_in_bytes(stream);
}

// write one bit. All values other than zero are considered to be a one-bit
void bitstream_write_bit(bitstream *stream, uint8_t bit) {

        if ( stream->currentByteIdx > bitstream_size_in_bytes(stream) ) {
            stream->error = true;
            return;
        }
        stream->bitsAvailable++;
        if ( bit )
        {
            stream->buffer[stream->currentByteIdx] |= stream->currentMask;
        }
        else
        {
            stream->buffer[stream->currentByteIdx] &= ~(stream->currentMask);
        }
        stream->currentMask >>= 1;
        if ( stream->currentMask == 0 )
        {
            stream->currentByteIdx++;
            stream->currentMask = 0x80;
        }
}

// read a single bit. Return value is either zero or one.
uint8_t bitstream_read_bit(bitstream *stream) {
        if ( stream->currentByteIdx >= bitstream_size_in_bytes(stream)  ) {
            stream->error = true;
            return 0;
        }
        uint8_t isSet = (stream->buffer[stream->currentByteIdx] & stream->currentMask) != 0;
        stream->currentMask >>= 1;
        if ( stream->currentMask == 0 )
        {
            stream->currentByteIdx++;
            stream->currentMask = 0x80;
        }
        return isSet;
}
