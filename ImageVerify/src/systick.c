#include "systick.h"
#include "stm32f103.h"

volatile uint32_t ticks_since_start;

void systick_init(void){

    SYSTICK_LOAD = 7999; //8MHz clock

    SYSTICK_CTRL |= (1 << 0) | (1 << 1) | (1 << 2); //Enable the SysTick and interrupt

}

uint32_t millis(void){
    return ticks_since_start;
}

void delay_ms(uint32_t ms){
    if (ms == 0){
        return;
    }
    
    uint32_t start = millis();
    while((millis() - start) < ms){
        __asm__ volatile ("nop");
    }
    return;
}

void SysTick_Handler(void){
    ticks_since_start++;
}