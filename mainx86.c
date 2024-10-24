#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ringbuffer.h>
#include <stdlib.h>

char tmp2[128];
uint8_t space[2];
ringbuffer buf;

uint8_t buf_write(uint8_t *data, uint8_t count) {
    uint8_t written = ringbuffer_bulk_write(&buf, data, count);
    printf("\r\nWritten: %d",written);

    uint8_t available = ringbuffer_available_bytes(&buf);
    printf("\r\nno. of bytes available to read: %d",available);

    if ( written != available ) {
      printf("\r\nERROR: Less available than we just read?");
      exit(0);
    }

    uint8_t remaining = ringbuffer_remaining_space(&buf);
    printf("\r\nRemaining space: %d",remaining);
    return written;
}

uint8_t buf_read() {
    uint8_t read= ringbuffer_bulk_read(&buf,&tmp2[0],sizeof(tmp2)-1);

    tmp2[read+1]=0;
    printf("\r\n########### %s #########",&tmp2[0]);

    printf("\r\nNum. of bytes read: %d",read);

    return read;
}

int main(int argc, char **argv)
{
    ringbuffer_init(&buf,&space[0],sizeof(space));

    char *data = "123456";
    char *ptr = data;
    uint8_t remaining = strlen(data);

    uint8_t read;
    do {
        if ( ringbuffer_is_empty(&buf) && remaining > 0) {
            uint8_t written = buf_write(ptr, remaining);
            remaining -= written;
            ptr += written;
            printf("\r\n---------------");
        }
        read = buf_read();
    } while(read>0);
}
