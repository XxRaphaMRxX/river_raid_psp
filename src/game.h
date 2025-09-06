// src/game.h
#ifndef GAME_H
#define GAME_H

#include "defines.h"

void init_game();
void update_game();
void draw_game();
void shutdown_game();

// Declaração para que main.c possa acessar
extern int game_over;

#endif