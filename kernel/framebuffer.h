#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
    uint32_t *address;
    int initialized;
} framebuffer_t;

void framebuffer_init(uint32_t address, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp);
void framebuffer_clear(uint32_t color);
void putpixel(int x, int y, uint32_t color);
void swap_buffers(void);
void clear_screen(uint32_t color);
uint32_t framebuffer_width(void);
uint32_t framebuffer_height(void);
uint32_t framebuffer_pitch(void);

#endif
