#include <stdint.h>
#include <serial.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include <debug_led.h>
#include <ringbuffer.h>
#include <stdio.h>

void main(void)
{
    debug_led_init();
    serial_init();

    // char tmp[128];
    // char tmp2[128];
    //
    // uint8_t space[2];
    // ringbuffer buf;
    //
    // ringbuffer_init(&buf,&space[0],sizeof(space));
    //
    // uint8_t written = ringbuffer_bulk_write(&buf,"a",1);
    // uint8_t printed = snprintf(&tmp[0],sizeof(tmp),"\r\nWritten: %d",written);
    // serial_send_sync( &tmp[0], printed );
    //
    // uint8_t available = ringbuffer_available_bytes(&buf);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nAvailable to read: %d",available);
    // serial_send_sync( &tmp[0], printed );
    //
    // uint8_t remaining = ringbuffer_remaining_space(&buf);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nRemaining space: %d",remaining);
    // serial_send_sync( &tmp[0], printed );
    //
    // uint8_t read= ringbuffer_bulk_read(&buf,&tmp2[0],sizeof(tmp2)-1);
    // tmp2[read+1]=0;
    //
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nNum. of bytes read: %d",read);
    // serial_send_sync( &tmp[0], printed );
    //
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nData: %s",&tmp2[0]);
    // serial_send_sync( &tmp[0], printed );
    //
    // // ----------------
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\n--------------------",&tmp2[0]);
    // serial_send_sync( &tmp[0], printed );
    //
    // written = ringbuffer_bulk_write(&buf,"b",1);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nWritten: %d",written);
    // serial_send_sync( &tmp[0], printed );
    //
    // available = ringbuffer_available_bytes(&buf);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nAvailable to reada: %d",available);
    // serial_send_sync( &tmp[0], printed );
    //
    // remaining = ringbuffer_remaining_space(&buf);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nRemaining space: %d",remaining);
    // serial_send_sync( &tmp[0], printed );
    //
    // read= ringbuffer_bulk_read(&buf,&tmp2[0],sizeof(tmp2)-1);
    // tmp2[read+1]=0;
    //
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nNum. of bytes read: %d",read);
    // serial_send_sync( &tmp[0], printed );
    //
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nData: %s",&tmp2[0]);
    // serial_send_sync( &tmp[0], printed );
    //
    // // ----------------
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\n--------------------",&tmp2[0]);
    // serial_send_sync( &tmp[0], printed );
    //
    // written = ringbuffer_bulk_write(&buf,"c",1);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nWritten: %d",written);
    // serial_send_sync( &tmp[0], printed );
    //
    // available = ringbuffer_available_bytes(&buf);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nAvailable to reada: %d",available);
    // serial_send_sync( &tmp[0], printed );
    //
    // remaining = ringbuffer_remaining_space(&buf);
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nRemaining space: %d",remaining);
    // serial_send_sync( &tmp[0], printed );
    //
    // read= ringbuffer_bulk_read(&buf,&tmp2[0],sizeof(tmp2)-1);
    // tmp2[read+1]=0;
    //
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nNum. of bytes read: %d",read);
    // serial_send_sync( &tmp[0], printed );
    //
    // printed = snprintf(&tmp[0],sizeof(tmp),"\r\nData: %s",&tmp2[0]);
    // serial_send_sync( &tmp[0], printed );

    while(true) {
        serial_send_async("1234 ", 5);
        _delay_ms(100);
    }
}
