#include "keyboard.h"
#include "ports.h"

static uint8_t g_key_states[128];

void keyboard_init(void)
{
    for (int i = 0; i < 128; i++)
    {
        g_key_states[i] = 0;
    }

    outb(0x64, 0xAE);
    io_wait();
}

void keyboard_update(void)
{
    while (keyboard_read_scancode() != 0)
    {
    }
}

uint8_t keyboard_read_scancode(void)
{
    if ((inb(0x64) & 0x01) == 0)
    {
        return 0;
    }

    uint8_t scancode = inb(0x60);

    if (scancode & 0x80)
    {
        g_key_states[scancode & 0x7F] = 0;
    }
    else
    {
        g_key_states[scancode] = 1;
    }

    return scancode;
}

int keyboard_is_key_down(uint8_t scancode)
{
    if (scancode >= 128)
    {
        return 0;
    }

    return g_key_states[scancode];
}
