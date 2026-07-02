#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

#define MOUSE_BUTTON_LEFT     0
#define MOUSE_BUTTON_RIGHT    1
#define MOUSE_BUTTON_MIDDLE   2
#define MOUSE_BUTTON_4        3
#define MOUSE_BUTTON_5        4
#define MOUSE_BUTTON_6        5
#define MOUSE_BUTTON_7        6
#define MOUSE_BUTTON_8        7
#define MOUSE_BUTTON_COUNT    8

extern int32_t mouse_x;
extern int32_t mouse_y;
extern int32_t mouse_dx;
extern int32_t mouse_dy;
extern uint8_t mouse_states[MOUSE_BUTTON_COUNT];
extern uint8_t mouse_button_pressed[MOUSE_BUTTON_COUNT];
extern uint8_t mouse_button_released[MOUSE_BUTTON_COUNT];

void mouse_init(void);
void mouse_update(void);
uint8_t mouse_read_scancode(void);
int mouse_is_button_down(uint8_t button);
int mouse_was_button_pressed(uint8_t button);
int mouse_was_button_released(uint8_t button);
void mouse_set_position(int32_t x, int32_t y);

#endif
