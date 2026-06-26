/*
 * SysTick mini-project
 *
 * - LED on PA0 blinks with delay_ms() (real milliseconds)
 * - UART prints uptime every second
 *
 *   make flash
 *   picocom -b 115200 /dev/ttyUSB0
 */
#include <stdint.h>
#include <stdbool.h>
#include "stm32f103.h"
#include "uart.h"
#include "systick.h"

#define DEBOUNCE_MS 30


static void led_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA_CRL &= ~(0xFU << 0);
    GPIOA_CRL |=  (0x2U << 0);
}

static void button_init(void){
    GPIOA_CRL &= ~(0xFU << 4);
    GPIOA_CRL |= (0x8U<< 4);

    GPIOA_ODR &= ~(1U << 1); //Pull down resisitor for button
}


static void led_set(bool on){
    if (on){
        GPIOA_BSRR = (1U << 0);
    }
    else{
        GPIOA_BSRR = (1U << 16);
    }
}

int main(void)
{
    led_init();
    button_init();
    systick_init();
    led_set(false);
    
    bool debounced_state = false;
    bool last_debounced_state = false;
    bool raw_state = false;
    bool armed = true;
    bool led_on = false;
    uint32_t change_start_ms = 0;



    for (;;){
        raw_state = (GPIOA_IDR >> 1) & 1; //Checking if the button is pressed or not, RAW
        if (raw_state != debounced_state){
            change_start_ms = millis();
            while ((millis() - change_start_ms) < DEBOUNCE_MS){
                __asm__ volatile ("nop");
                }

                if (((GPIOA_IDR >> 1) & 1) != debounced_state){
                    debounced_state = raw_state;
                }
            }
                
            if (debounced_state && !last_debounced_state && armed){
              led_on = !led_on;    
              led_set(led_on);
              armed = false;
            }

            if (!debounced_state){
                armed = true;
            }
            
        
        last_debounced_state = debounced_state;
    }
}
