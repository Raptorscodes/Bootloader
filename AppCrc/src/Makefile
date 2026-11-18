# Mini 7 — Application (slow blink), linked at 0x0800200C
# Header (12 bytes) is prepended by make_image.py → app_crc.bin @ 0x08002000
#
#   make
#   make image    → app_crc.bin
#   make flash    → writes app_crc.bin to 0x08002000 (ST-Link)
#   or sx -X app_crc.bin over UART to the ImageVerify bootloader

TARGET   = app
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size

CFLAGS   = -mcpu=cortex-m3 -mthumb \
           -Og -g -Wall -Wextra \
           -ffunction-sections -fdata-sections \
           -nostdlib -fno-builtin

LDFLAGS  = -T linker.ld -Wl,--gc-sections -nostdlib

SRCS = main.c startup.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET).bin

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

image: $(TARGET).bin
	python3 ../scripts/make_image.py

flash: image
	st-flash write app_crc.bin 0x08002000

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin app_crc.bin
