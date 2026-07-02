#include "timer.h"
#include "ports.h"

static volatile uint64_t g_ticks = 0;

/*
 * ============================================================================
 * PIT initialisieren
 * ============================================================================
 */

void timer_init(uint32_t frequency)
{
    if(frequency == 0)
        frequency = 100;

    uint16_t divisor =
        (uint16_t)(PIT_FREQUENCY / frequency);

    /*
     * Channel 0
     * Lobyte/Hibyte
     * Mode 3 (Square Wave)
     */

    outb(0x43, 0x36);

    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

/*
 * ============================================================================
 * Tick erhöhen
 * ============================================================================
 */

void timer_tick(void)
{
    g_ticks++;
}

/*
 * ============================================================================
 * Aktuelle Tickanzahl
 * ============================================================================
 */

uint32_t timer_ticks(void)
{
    return g_ticks;
}

static uint32_t div64_32(uint32_t high, uint32_t low, uint32_t divisor, uint32_t *remainder)
{
    uint32_t quotient;

    __asm__ volatile (
        "divl %4"
        : "=a"(quotient), "=d"(*remainder)
        : "a"(low), "d"(high), "r"(divisor)
    );

    return quotient;
}

timer_timepoint_t timer_elapsed(timer_timepoint_t start, timer_timepoint_t end)
{
    uint32_t low = end.low - start.low;
    uint32_t borrow = end.low < start.low;
    uint32_t high = end.high - start.high - borrow;

    return (timer_timepoint_t){
        .low = low,
        .high = high
    };
}

uint32_t timer_cycles_to_microseconds(timer_timepoint_t cycles, uint32_t cpu_mhz)
{
    uint32_t remainder;
    return div64_32(cycles.high, cycles.low, cpu_mhz, &remainder);
}

uint32_t timer_microseconds_elapsed(timer_timepoint_t start, timer_timepoint_t end, uint32_t cpu_mhz)
{
    timer_timepoint_t delta = timer_elapsed(start, end);
    return timer_cycles_to_microseconds(delta, cpu_mhz);
}

timer_timepoint_t timer_now(void)
{
    uint32_t low;
    uint32_t high;

    __asm__ volatile (
        "rdtsc"
        : "=a"(low), "=d"(high)
    );

    return (timer_timepoint_t){
        .low = low,
        .high = high
    };
}

/*
 * ============================================================================
 * Einfache Sleep-Funktion
 * ============================================================================
 */

void timer_sleep(uint32_t ticks)
{
    uint32_t end = g_ticks + ticks;

    while(g_ticks < end)
    {
        __asm__ volatile("hlt");
    }
}