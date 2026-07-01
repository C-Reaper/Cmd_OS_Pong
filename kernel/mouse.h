#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

extern int32_t mouse_x;
extern int32_t mouse_y;
extern uint8_t mouse_states[8];

void mouse_init(void);
void mouse_update(void);
uint8_t mouse_read_scancode(void);
int mouse_is_button_down(uint8_t button);
void mouse_set_position(int32_t x, int32_t y);

#endif
