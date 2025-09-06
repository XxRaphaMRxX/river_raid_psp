// src/graphics.c
#include "graphics.h"

void init_graphics() {
    sceDisplaySetMode(0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceDisplaySetFrameBuf(sceGeEdramGetAddr(), VRAM_BUFFER_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_IMMEDIATE);
}

void clear_screen(unsigned int color) {
    unsigned int* vram = (unsigned int*)sceGeEdramGetAddr();
    for (int i = 0; i < VRAM_BUFFER_WIDTH * SCREEN_HEIGHT; i++) {
        vram[i] = color;
    }
}

void draw_rect(int x, int y, int w, int h, unsigned int color) {
    unsigned int* vram = (unsigned int*)sceGeEdramGetAddr() + x + y * VRAM_BUFFER_WIDTH;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            vram[i] = color;
        }
        vram += VRAM_BUFFER_WIDTH;
    }
}

void swap_buffers() {
    sceDisplayWaitVblankStart();
}