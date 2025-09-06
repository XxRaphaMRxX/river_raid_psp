// src/enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "defines.h"

void init_enemies(Enemy enemies[]);
void spawn_enemy(Enemy enemies[], int river_center_x, int river_width, Sprite* sprite);
void update_enemies(Enemy enemies[]);
void draw_enemies(Enemy enemies[]);

#endif