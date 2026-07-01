#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

/*
 * ============================================================================
 * 8 Bit Port Input
 * ============================================================================
 */
static inline uint8_t inb(uint16_t port)
{
    uint8_t value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

/*
 * ============================================================================
 * 16 Bit Port Input
 * ============================================================================
 */
static inline uint16_t inw(uint16_t port)
{
    uint16_t value;

    __asm__ volatile (
        "inw %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

/*
 * ============================================================================
 * 32 Bit Port Input
 * ============================================================================
 */
static inline uint32_t inl(uint16_t port)
{
    uint32_t value;

    __asm__ volatile (
        "inl %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

/*
 * ============================================================================
 * 8 Bit Port Output
 * ============================================================================
 */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

/*
 * ============================================================================
 * 16 Bit Port Output
 * ============================================================================
 */
static inline void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile (
        "outw %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

/*
 * ============================================================================
 * 32 Bit Port Output
 * ============================================================================
 */
static inline void outl(uint16_t port, uint32_t value)
{
    __asm__ volatile (
        "outl %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

/*
 * ============================================================================
 * Small Delay
 * ============================================================================
 */
static inline void io_wait(void)
{
    __asm__ volatile (
        "outb %%al, $0x80"
        :
        : "a"(0)
    );
}

#endif