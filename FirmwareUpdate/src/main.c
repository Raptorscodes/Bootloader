/*
 * Mini 6 — firmware update bootloader
 *
 * Flow to implement:
 *   1. uart_init() and systick_init()
 *   2. Wait a few seconds for an upload to start
 *   3. If bytes arrive: receive the image (xmodem.c) and write it
 *      at APP_ADDRESS (0x08002000) using flash_erase_page / flash_write_word
 *      Never erase below 0x08002000 — that is this bootloader.
 *   4. If nothing arrives, or after a good receive: jump_to_app(APP_ADDRESS)
 *
 * Host (later):  sx -X app.bin < /dev/ttyUSB0 > /dev/ttyUSB0
 */
#include <stdint.h>
#include "stm32f103.h"
#include "uart.h"
#include "systick.h"
#include "flash.h"
#include "jump.h"
#include "xmodem.h"

int main(void)
{

    uart_init();
    systick_init();

    delay_ms(4000);

    start = millis();

    while(millis() - start < 4000){
        if(uart_has_data()){

            break;
    }
    else{
        jump_to_app(APP_ADDRESS);
    }
    

    for (;;) {
    }
}
}
