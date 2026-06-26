/*
 * main.c - UART Mini-Project demo
 *
 * What this does:
 *   1. Initializes USART1 at 115200 baud (TX=PA9, RX=PA10).
 *   2. Blinks the LED on PA0 (your existing wiring).
 *   3. Each blink cycle prints a "tick" message + counter over UART.
 *   4. Echoes back any characters typed in the serial terminal.
 *
 * To use:
 *   $ make flash
 *   $ picocom -b 115200 /dev/ttyUSB0
 *   (press reset, then type)
 */
#include <stdint.h>
#include "stm32f103.h"
#include "uart.h"

static void delay(volatile uint32_t n)
{
    while (n != 0) {
        __asm__ volatile ("nop");
        n = n - 1;
    }
}

static void led_init(void)
{
    /* GPIOA clock - safe to enable again even if uart_init already did */
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* PA0 as output 2 MHz push-pull (CNF=00, MODE=10 -> 0x2). */
    GPIOA_CRL &= ~(0xFU << 0);
    GPIOA_CRL |=  (0x2U << 0);
}

int main(void)
{
    led_init();
    uart_init();

    uart_puts("\r\n");
    uart_puts("================================================\r\n");
    uart_puts(" STM32F103 Blue Pill - Bare-metal UART demo\r\n");
    uart_puts(" USART1 @ 115200 8-N-1   LED on PA0\r\n");
    uart_puts(" Type characters - they'll be echoed back.\r\n");
    uart_puts("================================================\r\n\r\n");

    uint32_t counter = 0;

    for (;;) {
        /* LED on */
        GPIOA_BSRR = (1U << 0);
        delay(400000);

        /* LED off */
        GPIOA_BSRR = (1U << 16);
        delay(400000);

        uart_puts("tick ");
        uart_put_uint(counter++);

        /* Drain anything that came in while we were blinking. */
        while (uart_has_data()) {
            char c = uart_getc();
            uart_puts("   echo: '");
            uart_putc(c);
            uart_puts("'");
        }

        uart_puts("\r\n");
    }
}
