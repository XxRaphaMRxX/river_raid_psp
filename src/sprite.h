// src/sprite.h
#ifndef SPRITE_H
#define SPRITE_H

#include "defines.h"

int load_sprite(Sprite* sprite, const char* filename);
void draw_sprite(int sx, int sy, Sprite* sprite);
void free_sprite(Sprite* sprite);

#endif