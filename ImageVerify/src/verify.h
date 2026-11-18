#ifndef VERIFY_H
#define VERIFY_H

#include <stdint.h>

/* Must match AppCrc/scripts/make_image.py */
#define IMAGE_MAGIC        0xB00710ADU
#define IMAGE_HEADER_SIZE  12U

/* App slot is 56K starting at APP_ADDRESS; payload follows the 12-byte header. */
#define IMAGE_MAX_PAYLOAD  ((56U * 1024U) - IMAGE_HEADER_SIZE)

/* True (1) if magic/size/CRC at app_base all check out. */
int app_is_valid(uint32_t app_base);

/* Address of the vector table (payload), i.e. app_base + 12. */
uint32_t app_payload_address(uint32_t app_base);

#endif /* VERIFY_H */
