#include "verify.h"
#include "stm32f103.h"

/* Same CRC-32 as Python zlib.crc32 */
static uint32_t crc32(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFFU;

    for (uint32_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 1U) {
                crc = (crc >> 1) ^ 0xEDB88320U;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFFU;
}

static uint32_t read_u32(uint32_t addr)
{
    return *(volatile uint32_t *)addr;
}

uint32_t app_payload_address(uint32_t app_base)
{
    return app_base + IMAGE_HEADER_SIZE;
}

int app_is_valid(uint32_t app_base)
{
    uint32_t magic;
    uint32_t size;
    uint32_t stored_crc;
    uint32_t got_crc;
    uint32_t payload;
    uint32_t msp;

    magic = read_u32(app_base + 0U);
    size = read_u32(app_base + 4U);
    stored_crc = read_u32(app_base + 8U);

    if (magic != IMAGE_MAGIC) {
        return 0;
    }
    if (size == 0U || size > IMAGE_MAX_PAYLOAD) {
        return 0;
    }

    payload = app_payload_address(app_base);
    got_crc = crc32((const uint8_t *)payload, size);
    if (got_crc != stored_crc) {
        return 0;
    }

    /* First word of a Cortex-M image is the initial stack pointer (in SRAM). */
    msp = read_u32(payload);
    if ((msp & 0xFFFF0000U) != 0x20000000U) {
        return 0;
    }

    return 1;
}
