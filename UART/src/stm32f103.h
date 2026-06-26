/*
 * stm32f103.h - Memory-mapped register definitions for STM32F103 peripherals
 *
 * Covers the blocks used by this project: GPIOA, RCC (APB2 clock enables),
 * and USART1. Addresses and offsets match the STM32F103xx reference manual
 * (RM0008). All register accesses are volatile 32-bit loads/stores.
 */
#ifndef STM32F103_H
#define STM32F103_H

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/* GPIOA (port A)                                                             */
/* -------------------------------------------------------------------------- */

#define GPIOA_BASE      0x40010800U

#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00)) /* Port config low  (PA0–PA7)  */
#define GPIOA_CRH       (*(volatile uint32_t *)(GPIOA_BASE + 0x04)) /* Port config high (PA8–PA15) */
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x10)) /* Bit set/reset (atomic)      */

/* -------------------------------------------------------------------------- */
/* RCC — reset and clock control                                              */
/* -------------------------------------------------------------------------- */

#define RCC_BASE        0x40021000U
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18)) /* APB2 peripheral clock enable */

/* RCC_APB2ENR: set a bit to clock the corresponding block on APB2. */
#define RCC_APB2ENR_AFIOEN    (1U << 0)  /* Alternate-function I/O */
#define RCC_APB2ENR_IOPAEN    (1U << 2)  /* GPIO port A */
#define RCC_APB2ENR_USART1EN  (1U << 14) /* USART1 */

/* -------------------------------------------------------------------------- */
/* USART1                                                                     */
/* -------------------------------------------------------------------------- */

#define USART1_BASE     0x40013800U

#define USART1_SR       (*(volatile uint32_t *)(USART1_BASE + 0x00)) /* Status */
#define USART1_DR       (*(volatile uint32_t *)(USART1_BASE + 0x04)) /* Data (read RX / write TX) */
#define USART1_BRR      (*(volatile uint32_t *)(USART1_BASE + 0x08)) /* Baud rate (USARTDIV) */
#define USART1_CR1      (*(volatile uint32_t *)(USART1_BASE + 0x0C)) /* Control 1 */

/* USART1_SR status flags (read-only unless noted). */
#define USART_SR_RXNE   (1U << 5)  /* RX data ready in DR */
#define USART_SR_TC     (1U << 6)  /* Last frame finished on the wire */
#define USART_SR_TXE    (1U << 7)  /* DR empty, safe to write next byte */

/* USART1_CR1: enable receiver, transmitter, and the peripheral itself. */
#define USART_CR1_RE    (1U << 2)   /* Receiver enable */
#define USART_CR1_TE    (1U << 3)   /* Transmitter enable */
#define USART_CR1_UE    (1U << 13)  /* USART enable — set after pins and BRR */

#endif /* STM32F103_H */
