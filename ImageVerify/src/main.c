/*
 * Mini 7 — firmware update + image verify bootloader
 *
 * Flow:
 *   1. uart_init() / systick_init()
 *   2. xmodem_recieve(APP_ADDRESS) — may write a new app_crc.bin
 *   3. if app_is_valid(APP_ADDRESS): jump to payload (APP + 12)
 *   4. else stay here (do not jump into garbage)
 *
 * Host:
 *   cd AppCrc/src && make && python3 ../scripts/make_image.py
 *   stty -F /dev/ttyUSB0 115200 raw -echo
 *   sx -X app_crc.bin < /dev/ttyUSB0 > /dev/ttyUSB0
 */
#include <stdint.h>
#include "stm32f103.h"
#include "uart.h"
#include "systick.h"
#include "jump.h"
#include "xmodem.h"
#include "verify.h"

int main(void)
{
    uart_init();
    systick_init();

    (void)xmodem_recieve(APP_ADDRESS);

    if (app_is_valid(APP_ADDRESS)) {
        jump_to_app(app_payload_address(APP_ADDRESS));
    }

    for (;;) {
    }
}
