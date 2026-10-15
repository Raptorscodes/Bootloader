/*
 * Application main — Mini 5 skeleton
 *
 * TODO:
 *   1. Very early: SCB->VTOR = 0x08002000  (app vector table base)
 *   2. Init LED on PA0
 *   3. Blink slowly forever (different from bootloader's fast blink)
 *
 * Flash with:  make flash  (writes to 0x08002000)
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
