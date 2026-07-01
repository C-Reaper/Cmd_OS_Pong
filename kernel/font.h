#ifndef FONT_H
#define FONT_H

#include <stdint.h>

void font_char_render(uint8_t ch,int x,int y,uint32_t color);
void font_cstr_render(uint8_t* cstr,uint32_t size,int x,int y,uint32_t color);

void font_zoom_char_render(uint8_t ch,int x,int y,uint32_t mul,uint32_t color);
void font_zoom_cstr_render(uint8_t* cstr,uint32_t size,int x,int y,uint32_t mul,uint32_t color);

#endif
