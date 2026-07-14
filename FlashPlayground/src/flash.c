#include "flash.h"
#include "stm32f103.h"

void flash_unlock(void){
    FLASH_KEYR = FLASH_KEY1;
    FLASH_KEYR = FLASH_KEY2;
}

void flash_lock(void){
    FLASH_CR |= FLASH_CR_LOCK;
}

void flash_wait_busy(void){
    while(FLASH_SR & FLASH_SR_BSY){
        __asm__("nop");
    }
}

void flash_erase_page(uint32_t page){
    flash_wait_busy();
    flash_unlock();
    FLASH_CR |= FLASH_CR_PER;
    FLASH_AR = page;
    FLASH_CR |= FLASH_CR_STRT;
    flash_wait_busy();    
    FLASH_CR &= ~FLASH_CR_PER;
    flash_lock();
}


void flash_write_word(uint32_t address, uint16_t data){
    flash_wait_busy();
    flash_unlock();
    FLASH_CR |= FLASH_CR_PG;
    FLASH_AR = address;
    *(volatile uint16_t *)address = data;
    flash_wait_busy();
    FLASH_CR &= ~FLASH_CR_PG;
    flash_lock();
}