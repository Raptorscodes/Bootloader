/*
 * jump.c — hand control from bootloader to application
 *
 * app_addr is the base of the app image (usually APP_ADDRESS = 0x08002000).
 * The first two words there are the app vector table:
 *   [0] = initial Main Stack Pointer (MSP)
 *   [1] = Reset_Handler address
 */
#include <stdint.h>
#include "jump.h"
#include "stm32f103.h"

void jump_to_app(uint32_t app_addr)
{
    /* Function pointer type for the app's Reset_Handler (no args, no return). */
    typedef void (*app_reset_t)(void);

    uint32_t app_msp;
    uint32_t app_reset;
    app_reset_t app_entry;

    /* ------------------------------------------------------------------ */
    /* 1. Stop SysTick so the bootloader's SysTick_Handler can't fire     */
    /*    after we switch to the app.                                     */
    /* ------------------------------------------------------------------ */
    SYSTICK_CTRL = 0;
    SYSTICK_VAL  = 0;   /* clear current count */

    /* ------------------------------------------------------------------ */
    /* 2. NVIC: disable IRQs and clear any that are already pending.      */
    /*    Writing 1s affects all IRQ lines in bank 0.                     */
    /* ------------------------------------------------------------------ */
    NVIC_ICER0 = 0xFFFFFFFFU;  /* disable */
    NVIC_ICPR0 = 0xFFFFFFFFU;  /* clear pending */

    /* ------------------------------------------------------------------ */
    /* 3. Point the CPU at the APP vector table for future exceptions.    */
    /* ------------------------------------------------------------------ */
    SCB_VTOR = app_addr;

    /* ------------------------------------------------------------------ */
    /* 4–5. Read app stack pointer and Reset_Handler from vector table    */
    /* ------------------------------------------------------------------ */
    app_msp   = *(volatile uint32_t *)(app_addr + 0U);
    app_reset = *(volatile uint32_t *)(app_addr + 4U);

    app_entry = (app_reset_t)app_reset;

    /* ------------------------------------------------------------------ */
    /* 6. Switch to the app's stack, then call its Reset_Handler.         */
    /*    That runs app startup (.data/.bss) and then app main().         */
    /*    This call should never return.                                  */
    /* ------------------------------------------------------------------ */
    __asm volatile ("msr msp, %0" :: "r" (app_msp) : );
    app_entry();

    /* If we ever get here, something went wrong — hang. */
    for (;;) {
    }
}
