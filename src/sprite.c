// src/sprite.c
#include "sprite.h"
#include "graphics.h"
#include <stdio.h> // Para fopen

// A função load_sprite original vai aqui, com uma pequena modificação para retornar sucesso/falha
int load_sprite(Sprite* sprite, const char* filename) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    sprite->width = png_get_image_width(png_ptr, info_ptr);
    sprite->height = png_get_image_height(png_ptr, info_ptr);

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && png_get_bit_depth(png_ptr, info_ptr) < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
    if (png_get_bit_depth(png_ptr, info_ptr) == 16) png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    sprite->data = (unsigned int*)malloc(sprite->width * sprite->height * sizeof(unsigned int));
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * sprite->height);
    for (int y = 0; y < sprite->height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    for (int y = 0; y < sprite->height; y++) {
        png_byte* row = row_pointers[y];
        for (int x = 0; x < sprite->width; x++) {
            png_byte* ptr = &(row[x * 4]);
            sprite->data[y * sprite->width + x] = (ptr[3] << 24) | (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];
        }
    }

    for (int y = 0; y < sprite->height; y++) free(row_pointers[y]);
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 1;
}

void draw_sprite(int sx, int sy, Sprite* sprite) {
    if (!sprite || !sprite->data) return;
    unsigned int* vram = (unsigned int*)get_draw_buffer() + sx + sy * VRAM_BUFFER_WIDTH;
    for (int y = 0; y < sprite->height; y++) {
        for (int x = 0; x < sprite->width; x++) {
            unsigned int pixel = sprite->data[y * sprite->width + x];
            if ((pixel >> 24) > 128) { // Draw only if alpha > 50%
                vram[x] = pixel;
            }
        }
        vram += VRAM_BUFFER_WIDTH;
    }
}

void free_sprite(Sprite* sprite) {
    if (sprite && sprite->data) {
        free(sprite->data);
        sprite->data = NULL;
    }
}