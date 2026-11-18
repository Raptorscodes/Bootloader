/*
 * Application main — Mini 7 (AppCrc)
 *
 * Same blink demo as Mini 5/6. The new work is NOT in this file:
 *   ../scripts/make_image.py  — prepend/append magic + size + CRC
 *   ImageVerify/src/verify.c  — bootloader checks that header before jump
 *
 * Build:  make
 * Image:  python3 ../scripts/make_image.py   (after you write the script)
 * Send:   sx -X app_crc.bin < /dev/ttyUSB0 > /dev/ttyUSB0
 */
#include <stdint.h>
#include "stm32f103.h"

#define GPIOC_CRH   (*(volatile uint32_t *)(0x40011000U + 0x04))
#define GPIOC_BSRR  (*(volatile uint32_t *)(0x40011000U + 0x10))

static void delay(volatile uint32_t n)
{
    while (n != 0U) {
        __asm__ volatile ("nop");
        n = n - 1U;
    }
}

int main(void)
{
    SCB_VTOR = APP_ADDRESS;

    RCC_APB2ENR |= (1U << 4);
    GPIOC_CRH &= ~(0xFU << 20);
    GPIOC_CRH |=  (0x2U << 20);

    for (;;) {
        GPIOC_BSRR = (1U << 29);   /* on  */
        delay(800000);
        GPIOC_BSRR = (1U << 13);   /* off */
        delay(800000);
    }
}
