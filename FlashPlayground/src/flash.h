#include <stdint.h>

void flash_unlock(void);
void flash_lock(void);
void flash_wait_busy(void);
void flash_erase_page(uint32_t page);
void flash_write_word(uint32_t address, uint16_t data);