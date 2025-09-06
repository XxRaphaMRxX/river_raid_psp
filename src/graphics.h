// src/graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defines.h"

void init_graphics();
void clear_screen(unsigned int color);
void draw_rect(int x, int y, int w, int h, unsigned int color);
void swap_buffers();

#endif