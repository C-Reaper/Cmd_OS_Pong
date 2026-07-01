#include "renderer.h"
#include "framebuffer.h"

static int abs_i(int v)
{
    return (v < 0) ? -v : v;
}

void renderer_begin(uint32_t clearColor)
{
    clear_screen(clearColor);
}

void renderer_present(void)
{
    swap_buffers();
}

void draw_pixel(
    int x,
    int y,
    uint32_t color)
{
    putpixel(x, y, color);
}

void draw_line(
    int x0,
    int y0,
    int x1,
    int y1,
    uint32_t color)
{
    int dx = abs_i(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -abs_i(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        putpixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err * 2;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color)
{
    draw_line(x, y, x + width, y, color);
    draw_line(x, y, x, y + height, color);

    draw_line(
        x + width,
        y,
        x + width,
        y + height,
        color);

    draw_line(
        x,
        y + height,
        x + width,
        y + height,
        color);
}

void fill_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color)
{
    for (int yy = 0; yy < height; yy++)
    {
        for (int xx = 0; xx < width; xx++)
        {
            putpixel(
                x + xx,
                y + yy,
                color);
        }
    }
}

void draw_circle(
    int cx,
    int cy,
    int radius,
    uint32_t color)
{
    int x = radius;
    int y = 0;

    int err = 0;

    while (x >= y)
    {
        putpixel(cx + x, cy + y, color);
        putpixel(cx + y, cy + x, color);

        putpixel(cx - y, cy + x, color);
        putpixel(cx - x, cy + y, color);

        putpixel(cx - x, cy - y, color);
        putpixel(cx - y, cy - x, color);

        putpixel(cx + y, cy - x, color);
        putpixel(cx + x, cy - y, color);

        y++;

        if (err <= 0)
        {
            err += 2 * y + 1;
        }
        else
        {
            x--;
            err -= 2 * x + 1;
        }
    }
}

void fill_circle(
    int cx,
    int cy,
    int radius,
    uint32_t color)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
                putpixel(
                    cx + x,
                    cy + y,
                    color);
            }
        }
    }
}