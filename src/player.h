// src/player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "defines.h"

void init_player(Player* player, Sprite* sprite);
void handle_player_input(Player* player, SceCtrlData* pad);
void update_player_bounds(Player* player);
void draw_player(Player* player);

void init_bullets(Bullet bullets[]);
void fire_bullet(Player* player, Bullet bullets[]);
void update_bullets(Bullet bullets[]);
void draw_bullets(Bullet bullets[]);

#endif