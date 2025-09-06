// src/graphics.h
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "defines.h"

// Funções Públicas
void init_graphics();
void clear_screen(unsigned int color);
void draw_rect(int x, int y, int w, int h, unsigned int color);
void swap_buffers();
void* get_draw_buffer(); // <-- Nova função

#endif