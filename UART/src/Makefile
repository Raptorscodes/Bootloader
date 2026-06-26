# Build & flash the bare-metal UART demo for STM32F103 (Blue Pill).
# Run all targets from this directory:
#   make            # build firmware.bin
#   make flash      # build + flash via st-link
#   make monitor    # open picocom on /dev/ttyUSB0 at 115200
#   make clean

TARGET   = firmware
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size

CFLAGS   = -mcpu=cortex-m3 -mthumb \
           -Og -g -Wall -Wextra \
           -ffunction-sections -fdata-sections \
           -nostdlib -fno-builtin

LDFLAGS  = -T linker.ld -Wl,--gc-sections -nostdlib

SRCS = main.c startup.c uart.c
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
