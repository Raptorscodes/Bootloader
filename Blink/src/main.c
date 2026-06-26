#include <stdint.h>

#define RCC_APB2ENR  (*(volatile uint32_t *)0x40021018)
#define GPIOA_CRL    (*(volatile uint32_t *)0x40010800)
#define GPIOA_BSRR   (*(volatile uint32_t *)0x40010810)

static void delay(volatile uint32_t n)
{
    while (n != 0) {
        __asm__ volatile ("nop");
        n = n - 1;
    }
}

int main(void){

    RCC_APB2ENR |= (1 << 2); //Turning on the clock for GPIOA

    GPIOA_CRL &= ~(0xF << 0); 
    GPIOA_CRL |=  (0x2U << 0); //Setting the mode of PA0 to output

    for (;;) {
        GPIOA_BSRR = (1 << 0); //Setting the output of PA0 to high
        delay(100000);
        GPIOA_BSRR = (1 << 16); //Setting the output of PA0 to low
        delay(100000);
    
    }


}

 