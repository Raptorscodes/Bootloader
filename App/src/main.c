/*
 * Application main — Mini 5 skeleton
 *
 * TODO:
 *   1. Very early: SCB->VTOR = 0x08002000  (app vector table base)
 *   2. Init LED on PA0
 *   3. Blink slowly forever (different from bootloader's fast blink)
 *
 * Flash with:  make flash  (writes to 0x08002000)
 */
#include <stdint.h>
#include "stm32f103.h"

int main(void)
{
    SCB_VTOR = APP_ADDRESS;

    for (;;) {
    }
}
