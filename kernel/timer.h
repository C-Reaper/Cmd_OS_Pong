#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * ============================================================================
 * Programmable Interval Timer (PIT)
 * ============================================================================
 */

#define PIT_FREQUENCY 1193182

void timer_init(uint32_t frequency);

/*
 * Muss später einmal pro PIT-Interrupt aufgerufen werden.
 */
void timer_tick(void);

/*
 * Anzahl vergangener Ticks.
 */
uint64_t timer_ticks(void);

/*
 * Blockiert bis die gewünschte Anzahl Ticks vergangen ist.
 */
void timer_sleep(uint64_t ticks);

#endif