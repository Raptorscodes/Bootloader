#include "xmodem.h"
#include "uart.h"
#include "flash.h"
#include "systick.h"
#include "stm32f103.h"

#define SOH 0x01  /* start of 128-byte packet */
#define EOT 0x04  /* end of file */
#define ACK 0x06  /* packet OK, send next */
#define NAK 0x15  /* packet bad, send again */

/* Wait up to timeout_ms for one UART byte. Returns the byte, or -1. */
static int uart_getc_timeout(uint32_t timeout_ms)
{
    uint32_t start = millis();

    while ((millis() - start) < timeout_ms) {
        if (uart_has_data()) {
            return (uint8_t)uart_getc();
        }
    }
    return -1;
}

/* XMODEM-CRC: CRC-16 over the 128 data bytes (poly 0x1021, init 0). */
static uint16_t crc16(const uint8_t *buf, int len)
{
    uint16_t crc = 0;

    for (int i = 0; i < len; i++) {
        crc ^= (uint16_t)buf[i] << 8;
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x8000U) {
                crc = (uint16_t)((crc << 1) ^ 0x1021U);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int xmodem_recieve(uint32_t destination)
{
    uint8_t data[128];
    uint8_t expected = 1;   /* XMODEM block numbers start at 1 */
    int header = -1;

    /* Ask the PC to send. sx waits for 'C' before transmitting. */
    for (int tries = 0; tries < 10; tries++) {
        uart_putc('C');
        header = uart_getc_timeout(1000);
        if (header >= 0) {
            break;
        }
    }
    if (header < 0) {
        return -1;          /* no upload; leave flash alone */
    }

    while (header != EOT) {
        uint8_t block;
        uint8_t inv;
        uint8_t crc_hi;
        uint8_t crc_lo;
        uint16_t got;
        int good;

        if (header != SOH) {
            return -2;
        }

        block  = (uint8_t)uart_getc();
        inv    = (uint8_t)uart_getc();
        for (int i = 0; i < 128; i++) {
            data[i] = (uint8_t)uart_getc();
        }
        crc_hi = (uint8_t)uart_getc();
        crc_lo = (uint8_t)uart_getc();
        got = ((uint16_t)crc_hi << 8) | crc_lo;

        good = (inv == (uint8_t)~block) && (got == crc16(data, 128));

        /* Duplicate of last block: ACK again, do not rewrite flash. */
        if (good && block == (uint8_t)(expected - 1U)) {
            uart_putc(ACK);
        } else if (good && block == expected && destination >= APP_ADDRESS) {
            /* New page boundary (1 KB on F103) → erase first. */
            if ((destination & 0x3FFU) == 0U) {
                flash_erase_page(destination);
            }
            /* Flash programs 16-bit half-words, not bytes. */
            for (int i = 0; i < 128; i += 2) {
                uint16_t half = (uint16_t)data[i]
                              | ((uint16_t)data[i + 1] << 8);
                flash_write_word(destination + (uint32_t)i, half);
            }
            destination += 128U;
            expected++;
            uart_putc(ACK);
        } else {
            uart_putc(NAK);
        }

        header = uart_getc();
    }

    uart_putc(ACK);         /* ACK the EOT */
    return 0;
}