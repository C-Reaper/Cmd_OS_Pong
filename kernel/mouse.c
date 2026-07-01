#include "mouse.h"
#include "ports.h"

int32_t mouse_x = 0;
int32_t mouse_y = 0;
uint8_t mouse_states[8] = {0};

static uint8_t mouse_packet[3] = {0};
static uint8_t mouse_packet_index = 0;

void mouse_init(void)
{
    for (int i = 0; i < 8; i++)
    {
        mouse_states[i] = 0;
    }

    mouse_x = 0;
    mouse_y = 0;
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
    while (1)
    {
        if ((inb(0x64) & 0x01) == 0)
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

            mouse_states[0] = (mouse_packet[0] & 0x01) != 0;
            mouse_states[1] = (mouse_packet[0] & 0x02) != 0;
            mouse_states[2] = (mouse_packet[0] & 0x04) != 0;

            int8_t dx = (int8_t)mouse_packet[1];
            int8_t dy = (int8_t)mouse_packet[2];

            mouse_x += dx;
            mouse_y -= dy;
        }
    }

    if(mouse_x < 0)     mouse_x = 0;
    if(mouse_y < 0)     mouse_y = 0;
    if(mouse_x > 800)   mouse_x = 800;
    if(mouse_y < 600)   mouse_y = 600;
}

uint8_t mouse_read_scancode(void)
{
    if ((inb(0x64) & 0x01) == 0)
    {
        return 0;
    }

    return inb(0x60);
}

int mouse_is_button_down(uint8_t button)
{
    if (button >= 8)
    {
        return 0;
    }

    return mouse_states[button];
}

void mouse_set_position(int32_t x, int32_t y)
{
    mouse_x = x;
    mouse_y = y;
}
