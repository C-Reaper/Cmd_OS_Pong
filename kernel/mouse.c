#include "mouse.h"
#include "ports.h"
#include "framebuffer.h"

int32_t mouse_x = 0;
int32_t mouse_y = 0;
int32_t mouse_dx = 0;
int32_t mouse_dy = 0;
uint8_t mouse_states[MOUSE_BUTTON_COUNT] = {0};
uint8_t mouse_button_pressed[MOUSE_BUTTON_COUNT] = {0};
uint8_t mouse_button_released[MOUSE_BUTTON_COUNT] = {0};

static uint8_t mouse_packet[3] = {0};
static uint8_t mouse_packet_index = 0;

static void mouse_clear_button_events(void)
{
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
    {
        mouse_button_pressed[i] = 0;
        mouse_button_released[i] = 0;
    }
}

void mouse_init(void)
{
    mouse_clear_button_events();

    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
    {
        mouse_states[i] = 0;
    }

    mouse_x = 0;
    mouse_y = 0;
    mouse_dx = 0;
    mouse_dy = 0;
    mouse_packet_index = 0;

    outb(0x64, 0xA8);
    io_wait();

    outb(0x64, 0x20);
    io_wait();
    uint8_t status = inb(0x60);
    status |= 0x02;

    outb(0x64, 0x60);
    io_wait();
    outb(0x60, status);
    io_wait();

    outb(0x64, 0xD4);
    io_wait();
    outb(0x60, 0xF4);
    io_wait();
}

void mouse_update(void)
{
    mouse_clear_button_events();
    mouse_dx = 0;
    mouse_dy = 0;

    while (1)
    {
        uint8_t status = inb(0x64);

        if ((status & 0x01) == 0 || (status & 0x20) == 0)
        {
            break;
        }

        uint8_t byte = inb(0x60);

        if (mouse_packet_index == 0)
        {
            if ((byte & 0x08) == 0)
            {
                continue;
            }

            mouse_packet[0] = byte;
            mouse_packet_index = 1;
        }
        else if (mouse_packet_index == 1)
        {
            mouse_packet[1] = byte;
            mouse_packet_index = 2;
        }
        else
        {
            mouse_packet[2] = byte;
            mouse_packet_index = 0;

            for (uint8_t button = 0; button < 3; button++)
            {
                uint8_t new_state = (mouse_packet[0] & (1u << button)) != 0;
                uint8_t old_state = mouse_states[button];

                mouse_states[button] = new_state;
                mouse_button_pressed[button] = new_state && !old_state;
                mouse_button_released[button] = !new_state && old_state;
            }

            int8_t dx = (int8_t)mouse_packet[1];
            int8_t dy = (int8_t)mouse_packet[2];

            mouse_dx += dx;
            mouse_dy += dy;
            mouse_x += dx;
            mouse_y -= dy;
        }
    }

    if (mouse_x < 0)
    {
        mouse_x = 0;
    }

    if (mouse_y < 0)
    {
        mouse_y = 0;
    }

    uint32_t max_x = framebuffer_width() ? framebuffer_width() - 1 : 800;
    uint32_t max_y = framebuffer_height() ? framebuffer_height() - 1 : 600;

    if ((uint32_t)mouse_x > max_x)
    {
        mouse_x = max_x;
    }

    if ((uint32_t)mouse_y > max_y)
    {
        mouse_y = max_y;
    }
}

uint8_t mouse_read_scancode(void)
{
    uint8_t status = inb(0x64);

    if ((status & 0x01) == 0 || (status & 0x20) == 0)
    {
        return 0;
    }

    return inb(0x60);
}

int mouse_is_button_down(uint8_t button)
{
    if (button >= MOUSE_BUTTON_COUNT)
    {
        return 0;
    }

    return mouse_states[button];
}

int mouse_was_button_pressed(uint8_t button)
{
    if (button >= MOUSE_BUTTON_COUNT)
    {
        return 0;
    }

    return mouse_button_pressed[button];
}

int mouse_was_button_released(uint8_t button)
{
    if (button >= MOUSE_BUTTON_COUNT)
    {
        return 0;
    }

    return mouse_button_released[button];
}

void mouse_set_position(int32_t x, int32_t y)
{
    mouse_x = x;
    mouse_y = y;
}
