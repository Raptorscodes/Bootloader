/*
 * Bootloader main — Mini 5 skeleton
 *
 * TODO:
 *   1. Init LED (and optional UART / SysTick / button)
 *   2. Blink fast for ~2–3 seconds (or until button released)
 *   3. Call jump_to_app(APP_ADDRESS)  — implement in jump.c
 *
 * Memory: this image is linked at 0x08000000 (see linker.ld).
 * App:    linked/flashed at 0x08002000.
 */
#include <stdint.h>
#include "stm32f103.h"
#include "systick.h"
#include "uart.h"
#include "jump.h"

int main(void)
{
    /* TODO: led_init(); */
    /* TODO: uart_init(); systick_init(); */
    /* TODO: fast blink loop with delay_ms() */
    /* TODO: jump_to_app(APP_ADDRESS); */

    for (;;) {
    }
}
