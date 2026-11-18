# Mini 7 — image verify bootloader
# Lives at 0x08000000. Receives app via XMODEM, then only jumps if
# the image header (magic + size + CRC) checks out.
#
#   make
#   make flash
#   make monitor

TARGET   = verify
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size

CFLAGS   = -mcpu=cortex-m3 -mthumb \
           -Og -g -Wall -Wextra \
           -ffunction-sections -fdata-sections \
           -nostdlib -fno-builtin

LDFLAGS  = -T linker.ld -Wl,--gc-sections -nostdlib

SRCS = main.c startup.c uart.c systick.c flash.c jump.c xmodem.c verify.c
OBJS = $(SRCS:.c=.o)

PORT     ?= /dev/ttyUSB0
BAUD     ?= 115200

all: $(TARGET).bin

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

monitor:
	picocom -b $(BAUD) $(PORT)

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin
