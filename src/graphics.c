// src/graphics.c
#include "graphics.h"
#include <pspdisplay.h>

// Endereço da VRAM acessível pela CPU
#define VRAM_ADDR_CPU (void*)0x44000000

static void* g_framebuffers[2];
static int g_current_buffer_index;

void init_graphics() {
    g_framebuffers[0] = VRAM_ADDR_CPU;
    g_framebuffers[1] = (void*)((unsigned int)VRAM_ADDR_CPU + VRAM_BUFFER_WIDTH * SCREEN_HEIGHT * 4);
    g_current_buffer_index = 0;
    
    sceDisplaySetMode(0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Inicia exibindo o buffer 0, enquanto o desenho ocorrerá no buffer 1.
    sceDisplaySetFrameBuf(g_framebuffers[0], VRAM_BUFFER_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_IMMEDIATE);
}

void* get_draw_buffer() {
    // Retorna o buffer que NÃO está sendo exibido (o back buffer).
    return g_framebuffers[g_current_buffer_index ^ 1];
}

void clear_screen(unsigned int color) {
    void* draw_buffer = get_draw_buffer();
    unsigned int* vram = (unsigned int*)draw_buffer;
    for (int i = 0; i < VRAM_BUFFER_WIDTH * SCREEN_HEIGHT; i++) {
        vram[i] = color;
    }
}

void draw_rect(int x, int y, int w, int h, unsigned int color) {
    void* draw_buffer = get_draw_buffer();
    
    // Verificação de limites para não desenhar fora da tela
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT || x + w <= 0 || y + h <= 0) return;
    int x_start = x > 0 ? x : 0;
    int y_start = y > 0 ? y : 0;
    int x_end = (x + w < SCREEN_WIDTH) ? (x + w) : SCREEN_WIDTH;
    int y_end = (y + h < SCREEN_HEIGHT) ? (y + h) : SCREEN_HEIGHT;

    unsigned int* vram_start = (unsigned int*)draw_buffer + x_start + y_start * VRAM_BUFFER_WIDTH;
    for (int j = y_start; j < y_end; j++) {
        unsigned int* vram_line = vram_start;
        for (int i = x_start; i < x_end; i++) {
            *vram_line++ = color;
        }
        vram_start += VRAM_BUFFER_WIDTH;
    }
}

void swap_buffers() {
    void* draw_buffer = get_draw_buffer();
    sceDisplayWaitVblankStart();
    g_current_buffer_index ^= 1; // Alterna o buffer (0 -> 1, 1 -> 0)
    // Manda o hardware de vídeo exibir o buffer em que acabamos de desenhar.
    sceDisplaySetFrameBuf(draw_buffer, VRAM_BUFFER_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_NEXTFRAME);
}