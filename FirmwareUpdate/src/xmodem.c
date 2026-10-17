#include "xmodem.h"
#include "uart.h"
#include "stm32f103.h"

void xmodem_recieve(uint32_t destination){
    
    uint8_t header;
    uint8_t data[128];
    uint8_t block;
    uart_putc('C'); //Send the control character 'C' to the terminal to indicate that the xmodem protocol is starting
    header = uart_getc();

    while(header != 0x04){

        if(header != 0x01){
            return;
        }
        block = uart_getc();
        uart_getc();

        for(int i =0; i < 128; i++){
            data[i] = uart_getc();
        }

        uart_getc();
        uart_getc();
        uart_putc(0x06);

        header = uart_getc();
    }

    uart_putc(0x06);


}