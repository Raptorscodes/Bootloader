#include "uart.h"
#include "stm32f103.h"

void uart_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN
                |  RCC_APB2ENR_AFIOEN
                |  RCC_APB2ENR_USART1EN;

    GPIOA_CRH &= ~((0xFU << 4) | (0xFU << 8));
    GPIOA_CRH |=  ((0xBU << 4) | (0x4U << 8));

    USART1_BRR = (4U << 4) | 5U;
    USART1_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void uart_putc(char c)
{
    while (!(USART1_SR & USART_SR_TXE)) {
        __asm__ volatile ("nop");
    }
    USART1_DR = (uint8_t)c;
}

void uart_puts(const char *s)
{
    while (*s != '\0') {
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

int uart_has_data(void)
{
    return (USART1_SR & USART_SR_RXNE) != 0;
}

char uart_getc(void)
{
    while (!(USART1_SR & USART_SR_RXNE)) {
        __asm__ volatile ("nop");
    }
    return (char)(USART1_DR & 0xFF);
}

void uart_put_uint(uint32_t n)
{
    if (n >= 10U) {
        uart_put_uint(n / 10U);
    }
    uart_putc((char)('0' + (n % 10U)));
}
