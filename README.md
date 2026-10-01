# Bootloader Firmware Development Workspace

This repository is a bare-metal firmware workspace for the STM32F103 Blue Pill (STM32F103C8T6). It is designed to teach and practice the core principles behind embedded bootloader development, including UART communication, system timing, GPIO control, and direct FLASH programming.

The project is intentionally structured as a series of small experiments that build toward the behavior of a real bootloader: initializing the MCU, communicating with a host, erasing and writing application memory, and preparing to jump to a firmware image.

---

## Project Overview

This workspace includes several firmware examples that progressively introduce the concepts needed for bootloader design:

- Blink  
  Basic LED toggling on the Blue Pill.

- UART  
  Serial communication using USART1 at 115200 baud.

- Systick  
  Timer-based delays and timing control.

- FlashPlayground  
  Practical FLASH erase, write, and read tests using a scratch page in memory.

These modules provide a hands-on foundation for understanding how a bootloader interacts with the MCU and external flash memory.

---

## Hardware Target

- STM32F103 Blue Pill development board
- 3.3V power supply
- USB-to-serial adapter for UART communication
- ST-Link or compatible SWD programmer for flashing firmware

---

## Toolchain Requirements

Before building the firmware, ensure the following tools are installed:

- arm-none-eabi-gcc
- arm-none-eabi-objcopy
- arm-none-eabi-size
- st-flash
- picocom or another serial monitor

On Debian/Ubuntu-based systems, the toolchain can typically be installed with:

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi stlink-tools picocom
```

---

## Repository Structure

```text
Bootloader/
├── Blink/
│   └── src/
├── UART/
│   └── src/
├── Systick/
│   └── src/
├── FlashPlayground/
│   └── src/
├── README.md
└── .gitignore
```

Each folder contains the source files and linker configuration for a standalone firmware example.

---

## Building the Firmware

Navigate to the project directory and build the target:

```bash
cd FlashPlayground/src
make
```

This produces a firmware ELF and binary image for the Blue Pill.

---

## Flashing to the Board

To flash the compiled image via ST-Link:

```bash
make flash
```

---

## Monitoring Serial Output

To open the serial console at 115200 baud:

```bash
make monitor
```

If your serial device is not on /dev/ttyUSB0, pass the port explicitly:

```bash
PORT=/dev/ttyS0 make monitor
```

---

## Flash Playground Behavior

The FlashPlayground project demonstrates the same low-level operations used by an embedded bootloader:

- erase a flash page
- write 16-bit values to flash memory
- read the contents back for verification
- use a safe scratch page outside the application area

The scratch page is defined as:

```c
#define SCRATCH_PAGE 0x0800FC00
```

This address is intentionally placed away from the main program image so that testing does not overwrite the active firmware.

The UART menu supports:

- e — erase the page
- r — erase and write a pattern
- d — dump memory contents

---

## Flash Programming Principles

Direct FLASH programming on STM32 devices requires careful handling:

- FLASH must be unlocked before erase or programming.
- The controller must wait for the busy flag to clear before new operations.
- Erasing resets bits to 1; writing only changes 1 to 0.
- A valid page address and proper register sequencing are required for reliable operation.

These principles are the foundation for firmware update logic in a bootloader.

---

## Typical Bootloader Workflow

A real bootloader generally follows this flow:

1. Initialize clocks and peripheral hardware
2. Configure UART for communication
3. Receive a firmware image or command
4. Unlock the flash controller
5. Erase application sectors
6. Program the incoming data
7. Verify the image
8. Jump to the new application reset vector

This project provides the practical building blocks required to implement that process safely and predictably.

---

## Safety Notes

- Never erase or overwrite code that is currently executing.
- Use a dedicated scratch page or safe memory region for testing.
- Verify addresses carefully before writing to FLASH.
- Always test bootloader behavior with a controlled and reversible firmware flow.

---

## Summary

This project is intended as a learning and experimentation platform for embedded bootloader development on the STM32F103 Blue Pill. It covers the essential hardware and firmware concepts needed to build a functional bootloader from the ground up.
