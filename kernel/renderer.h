#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

void renderer_begin(uint32_t clearColor);

void renderer_present(void);

void draw_pixel(
    int x,
    int y,
    uint32_t color);

void draw_line(
    int x0,
    int y0,
    int x1,
    int y1,
    uint32_t color);

void draw_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color);

void fill_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color);

void draw_circle(
    int cx,
    int cy,
    int radius,
    uint32_t color);

void fill_circle(
    int cx,
    int cy,
    int radius,
    uint32_t color);

#endif