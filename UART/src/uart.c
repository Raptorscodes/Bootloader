#include "uart.h"
#include "stm32f103.h"

void uart_init(void){
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN; //Turning on the clock for GPIOA and USART1 using bitmaks

    //Inilatizing both pin 9 and 10 to alternate function 1 TX and RX for UART
    GPIOA_CRH &= ~(0xF << 4); //Clearing the 4 bits for Pin 9 (TX)
    GPIOA_CRH |= (0xB << 4); //Setting the 4 bits for Pin 9 (TX) to alternate function 1 TX
    
    GPIOA_CRH &= ~(0xF << 8);
    GPIOA_CRH |= (0x4 << 8); //Setting the 4 bits for Pin 10 (RX) to alternate function 1 RX


    //Setting the baud rate to 115200, divisor is ~4.43

    USART1_BRR |= (0x004 << 4);
    USART1_BRR |= (0x5 << 0);

    //Enabling the transmitter and receiver
    USART1_CR1 |= USART_CR1_RE | USART_CR1_TE;
    USART1_CR1 |= USART_CR1_UE;

}


void uart_putc(char c){

    while(!(USART1_SR & USART_SR_TXE)){
        __asm__ volatile ("nop");
    }

    USART1_DR = (uint8_t)c;

}

void uart_puts(const char *c){
    while(*c != '\0'){
        uart_putc(*c);
        c++;
    }
}

int uart_has_data(void){

    return (USART1_SR & USART_SR_RXNE);
}


char uart_getc(void){
    while(!(USART1_SR & USART_SR_RXNE)){
        __asm__ volatile ("nop");
    }

    return (char)USART1_DR;
}

void uart_put_uint(uint32_t n)
{
    if (n >= 10U) {
        uart_put_uint(n / 10U);
    }
    uart_putc((char)('0' + (n % 10U)));
}