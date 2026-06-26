TARGET   = firmware
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size
CFLAGS   = -mcpu=cortex-m3 -mthumb \
           -Og -g -Wall -Wextra \
           -ffunction-sections -fdata-sections \
           -nostdlib -fno-builtin
LDFLAGS  = -T linker.ld -Wl,--gc-sections -nostdlib
SRCS = src/main.c src/startup.c
OBJS = $(SRCS:.c=.o)
all: $(TARGET).bin
$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin