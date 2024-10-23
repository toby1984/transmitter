#include <stdint.h>
#include <serial.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include <debug_led.h>

void main(void)
{
    debug_led_init();
    serial_init();

    char *data = "1234 ";
    while(true) {
        serial_send_async( (uint8_t*) data, strlen(data) );
    }
}
