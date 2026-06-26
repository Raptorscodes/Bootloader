/*
 * startup.c - Reset vector, vector table, and minimal C runtime init
 *
 * Same idea as your Blink startup but with two upgrades:
 *   1. A real HardFault_Handler that lights PA0 solid as a crash beacon.
 *      (Once you have a real bootloader, this is where you'd dump
 *       stacked registers over UART for debugging.)
 *   2. All 16 Cortex-M3 system exception slots are present (some unused,
 *      reserved by ARM, kept for layout correctness).
 */
#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

int  main(void);
void Default_Handler(void);
void Reset_Handler(void);
void HardFault_Handler(void);

void Reset_Handler(void)
{
    /* Copy .data from FLASH to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero .bss */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();

    for (;;) { /* main() returned - park here */ }
}

void Default_Handler(void)
{
    for (;;) { /* unhandled IRQ - spin */ }
}

/* On a HardFault, light PA0 LED solid and spin.
   GPIOA clock + pin mode were already set in main(); if the fault
   happened before that, the LED simply won't light - that's still a
   useful clue. */
void HardFault_Handler(void)
{
    #define BSRR_ADDR (*(volatile uint32_t *)0x40010810)
    BSRR_ADDR = (1U << 0);
    for (;;) { }
}

__attribute__((section(".isr_vector"), used))
void (* const g_vector_table[])(void) = {
    /* ----- Cortex-M3 system exceptions ----- */
    (void (*)(void)) &_estack,   /*  0: Initial Main Stack Pointer */
    Reset_Handler,               /*  1: Reset                      */
    Default_Handler,             /*  2: NMI                        */
    HardFault_Handler,           /*  3: HardFault                  */
    Default_Handler,             /*  4: MemManage                  */
    Default_Handler,             /*  5: BusFault                   */
    Default_Handler,             /*  6: UsageFault                 */
    0, 0, 0, 0,                  /*  7-10: Reserved                */
    Default_Handler,             /* 11: SVCall                     */
    Default_Handler,             /* 12: DebugMon                   */
    0,                           /* 13: Reserved                   */
    Default_Handler,             /* 14: PendSV                     */
    Default_Handler,             /* 15: SysTick                    */
    /* External (peripheral) IRQs are not used yet.
       Add them here once you start using EXTI / USART RX interrupts. */
};
