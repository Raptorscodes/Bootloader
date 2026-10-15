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
#include <stdbool.h>


    /* TODO: led_init(); */
    /* TODO: uart_init(); systick_init(); */
    /* TODO: fast blink loop with delay_ms() */
    /* TODO: jump_to_app(APP_ADDRESS); */

    #define GPIOC_BASE  0x40011000U
    #define GPIOC_CRH   (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
    #define GPIOC_BSRR  (*(volatile uint32_t *)(GPIOC_BASE + 0x10))
    #define RCC_IOPCEN  (1U << 4)
    
    static void led_init(void)
    {
        RCC_APB2ENR |= RCC_IOPCEN;
        GPIOC_CRH &= ~(0xFU << 20);  /* PC13 */
        GPIOC_CRH |=  (0x2U << 20);  /* output, 2 MHz, push-pull */
    }
    
    static void led_set(bool on)
    {
        if (on) {
            GPIOC_BSRR = (1U << 29); /* PC13 low  = LED on  */
        } else {
            GPIOC_BSRR = (1U << 13); /* PC13 high = LED off */
        }
    }

int main(void){

    uint32_t elapsed = 0;

    led_init();
    systick_init();

    while (elapsed < 20000U) {
        led_set(true);
        delay_ms(100U);
        led_set(false);
        delay_ms(100U);
        elapsed += 2U * 100U;
    }
    led_set(false);
    jump_to_app(APP_ADDRESS);
    for (;;) {
    }
}


