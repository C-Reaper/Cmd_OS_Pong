#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * ============================================================================
 * Programmable Interval Timer (PIT)
 * ============================================================================
 */

#define PIT_FREQUENCY 1193182

typedef struct timer_timepoint
{
    uint32_t low;
    uint32_t high;
} timer_timepoint_t;

void timer_init(uint32_t frequency);

/*
 * Muss später einmal pro PIT-Interrupt aufgerufen werden.
 */
void timer_tick(void);

/*
 * Anzahl vergangener Ticks.
 */
uint32_t timer_ticks(void);

/*
 * Gibt den aktuellen Zeitstempel zurück (Taktzyklus-Zähler).
 */
timer_timepoint_t timer_now(void);

/*
 * Differenz zwischen zwei Zeitpunkten in Taktzyklen.
 */
timer_timepoint_t timer_elapsed(timer_timepoint_t start, timer_timepoint_t end);

/*
 * Wandelt CPU-Taktzyklen in Mikrosekunden um. cpu_mhz muss die Taktfrequenz in MHz sein.
 */
uint32_t timer_cycles_to_microseconds(timer_timepoint_t cycles, uint32_t cpu_mhz);

/*
 * Differenz zwischen zwei Zeitpunkten in Mikrosekunden.
 */
uint32_t timer_microseconds_elapsed(timer_timepoint_t start, timer_timepoint_t end, uint32_t cpu_mhz);

/*
 * Blockiert bis die gewünschte Anzahl Ticks vergangen ist.
 */
void timer_sleep(uint32_t ticks);

#endif