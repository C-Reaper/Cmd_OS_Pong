#include <stdint.h>

#include "framebuffer.h"
#include "keyboard.h"
#include "mouse.h"
#include "pong.h"
#include "timer.h"
#include "fpu.h"

struct multiboot_tag_framebuffer
{
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved[6];
};

static struct multiboot_tag_framebuffer *find_framebuffer_tag(uint32_t mb_info_addr)
{
    if (mb_info_addr == 0)
    {
        return 0;
    }

    uint32_t *ptr = (uint32_t *)mb_info_addr;
    uint32_t total_size = ptr[0];
    uint8_t *tag = (uint8_t *)(ptr + 2);

    while ((uint32_t)(tag - (uint8_t *)ptr) < total_size)
    {
        uint32_t tag_type = ((uint32_t *)tag)[0];
        uint32_t tag_size = ((uint32_t *)tag)[1];

        if (tag_type == 8)
        {
            return (struct multiboot_tag_framebuffer *)tag;
        }

        if (tag_size == 0)
        {
            break;
        }

        tag += (tag_size + 7) & ~7u;
    }

    return 0;
}

void kernel_main(uint32_t magic, uint32_t mb_info_addr)
{
    (void)magic;

    struct multiboot_tag_framebuffer *fb_tag = find_framebuffer_tag(mb_info_addr);

    if (fb_tag != 0)
    {
        framebuffer_init(
            (uint32_t)fb_tag->framebuffer_addr,
            fb_tag->framebuffer_width,
            fb_tag->framebuffer_height,
            fb_tag->framebuffer_pitch,
            fb_tag->framebuffer_bpp);
    }
    else
    {
        framebuffer_init(0, 800, 600, 3200, 32);
    }

    fpu_init();
    timer_init(100);
    keyboard_init();
    mouse_init();

    pong_init();
    pong_run();
}
