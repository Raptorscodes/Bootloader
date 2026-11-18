#include <stdint.h>
#include "stm32f103.h"
#include "uart.h"
#include "systick.h"
#include "jump.h"
#include "xmodem.h"

int main(void)
{
    int rc;

    uart_init();
    systick_init();

    rc = xmodem_recieve(APP_ADDRESS);

    if (rc == 0 || rc == -1) {
        jump_to_app(APP_ADDRESS);
    }

    for (;;) {
    }
}