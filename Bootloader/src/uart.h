#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
int  uart_has_data(void);
void uart_put_uint(uint32_t n);

#endif /* UART_H */
