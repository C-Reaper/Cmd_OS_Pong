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

uint64_t timer_ticks(void)
{
    return g_ticks;
}

/*
 * ============================================================================
 * Einfache Sleep-Funktion
 * ============================================================================
 */

void timer_sleep(uint64_t ticks)
{
    uint64_t end = g_ticks + ticks;

    while(g_ticks < end)
    {
        __asm__ volatile("hlt");
    }
}