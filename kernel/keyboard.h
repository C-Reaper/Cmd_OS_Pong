#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEY_ESC 0x01
#define KEY_W 0x11
#define KEY_S 0x1F
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_ENTER 0x1C

void keyboard_init(void);
void keyboard_update(void);
uint8_t keyboard_read_scancode(void);
int keyboard_is_key_down(uint8_t scancode);

#endif
