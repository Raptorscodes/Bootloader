#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

int  main(void);
void Default_Handler(void);
void Reset_Handler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();

    for (;;) {}
}

void Default_Handler(void)
{
    for (;;) {}
}

void HardFault_Handler(void)
{
    #define BSRR_ADDR (*(volatile uint32_t *)0x40010810)
    BSRR_ADDR = (1U << 0);
    for (;;) {}
}

__attribute__((section(".isr_vector"), used))
void (* const g_vector_table[])(void) = {
    (void (*)(void)) &_estack,
    Reset_Handler,
    Default_Handler,
    HardFault_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    0, 0, 0, 0,
    Default_Handler,
    Default_Handler,
    0,
    Default_Handler,
    SysTick_Handler,   /* slot 15: SysTick — was Default_Handler in UART */
};
