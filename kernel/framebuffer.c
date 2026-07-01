#include "framebuffer.h"
#include "memcpy.h"

static uint32_t pbuffer[800 * 600];
static uint32_t* b_framebuffer;
static framebuffer_t g_framebuffer = {0};

static uint32_t framebuffer_index(uint32_t x, uint32_t y)
{
    return y * (g_framebuffer.pitch / 4) + x;
}

void framebuffer_init(uint32_t address, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp)
{
    g_framebuffer.address = (uint32_t *)address;
    g_framebuffer.width = width;
    g_framebuffer.height = height;
    g_framebuffer.pitch = pitch;
    g_framebuffer.bpp = bpp;
    g_framebuffer.initialized = (address != 0 && width != 0 && height != 0);

    if (g_framebuffer.initialized)
    {
        framebuffer_clear(0x00000000);
    }

    b_framebuffer = pbuffer;
}

void framebuffer_clear(uint32_t color)
{
    if (!g_framebuffer.initialized || g_framebuffer.address == 0)
    {
        return;
    }

    for (uint32_t y = 0; y < g_framebuffer.height; y++)
    {
        for (uint32_t x = 0; x < g_framebuffer.width; x++)
        {
            ((uint32_t *)b_framebuffer)[framebuffer_index(x, y)] = color;
        }
    }
}

void putpixel(int x, int y, uint32_t color)
{
    if (!g_framebuffer.initialized || b_framebuffer == 0)
    {
        return;
    }

    if (x < 0 || y < 0)
    {
        return;
    }

    if ((uint32_t)x >= g_framebuffer.width || (uint32_t)y >= g_framebuffer.height)
    {
        return;
    }

    ((uint32_t *)b_framebuffer)[framebuffer_index((uint32_t)x, (uint32_t)y)] = color;
}

void swap_buffers(void)
{
    memcpy(
        g_framebuffer.address,
        b_framebuffer,
        sizeof(uint32_t) * g_framebuffer.width * g_framebuffer.height
    );
}

void clear_screen(uint32_t color)
{
    framebuffer_clear(color);
}

uint32_t framebuffer_width(void)
{
    return g_framebuffer.width;
}

uint32_t framebuffer_height(void)
{
    return g_framebuffer.height;
}

uint32_t framebuffer_pitch(void)
{
    return g_framebuffer.pitch;
}
