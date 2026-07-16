/*
 * main.c — FLASH playground UART menu
 *
 * Lets you practice erase / write / read of a scratch FLASH page from the
 * Blue Pill itself (the same operations a bootloader will use later).
 *
 * Commands (type in picocom @ 115200):
 *   e  — erase SCRATCH_PAGE to all 0xFF
 *   w  — erase, then write a short test pattern of half-words
 *   d  — dump (read) the start of that page as hex
 *
 * Scratch page is SCRATCH_PAGE in stm32f103.h (e.g. 0x0800FC00) — far from
 * the code at 0x08000000 so we do not erase our own program.
 *
 *   make flash
 *   picocom -b 115200 /dev/ttyUSB0
 */
#include <stdint.h>
#include "stm32f103.h"
#include "flash.h"
#include "uart.h"

/*
 * Print one 16-bit value as 4 hex digits (no "0x" prefix).
 * Example: 0x1111 → "1111"
 * We don't have printf; this is a tiny hex printer for dumps.
 */
static void put_hex16(uint16_t v)
{
    static const char hex[] = "0123456789ABCDEF";
    uart_putc(hex[(v >> 12) & 0xF]); /* high nibble */
    uart_putc(hex[(v >>  8) & 0xF]);
    uart_putc(hex[(v >>  4) & 0xF]);
    uart_putc(hex[(v >>  0) & 0xF]); /* low nibble */
}

/*
 * Read the first 16 half-words of the scratch page and print them.
 * Reading FLASH needs no unlock — it is normal memory-mapped load.
 */
static void dump_page(void)
{
    uart_puts("dump:\n");

    for (uint32_t i = 0; i < 16U; i++) {
        /* Half-word addresses step by 2: +0, +2, +4, ... */
        uint32_t addr = SCRATCH_PAGE + (i * 2U);
        uint16_t value = *(volatile uint16_t *)addr;

        put_hex16(value);
        uart_putc(' ');

        /* Newline every 8 values so the terminal stays readable */
        if ((i % 8U) == 7U) {
            uart_puts("\n");
        }
    }
    uart_puts("\n");
}

int main(void)
{
    /* Bring up USART1 (PA9 TX / PA10 RX @ 115200) */
    uart_init();

    uart_puts("\nFlash Playground\n");
    uart_puts("e=erase  r=write  d=dump\n");

    for (;;) {
        /* Block until the PC sends one character */
        char c = uart_getc();

        if (c == 'e') {
            /* Wipe the whole 1 KB scratch page → all bytes become 0xFF */
            flash_erase_page(SCRATCH_PAGE);
            uart_puts("erased\n");
        }
        else if (c == 'r') {
            /*
             * Always erase before writing a new pattern.
             * FLASH can only turn 1→0; erase restores 0xFFFF cells.
             */
            flash_erase_page(SCRATCH_PAGE);

            /*
             * Write 8 half-words of a known pattern:
             *   addr+0 → 0x1111, addr+2 → 0x2222, ... addr+14 → 0x8888
             * Address must stay even (i * 2).
             */
            for (uint32_t i = 0; i < 8U; i++) {
                uint32_t addr = SCRATCH_PAGE + (i * 2U);
                uint16_t data = (uint16_t)((i + 1U) * 0x1111U);
                flash_write_word(addr, data);
            }

            uart_puts("written\n");
        }
        else if (c == 'd') {
            /* Show what is currently stored (survives reset if it was written) */
            dump_page();
        }
        /* Unknown characters are ignored */
    }
}
