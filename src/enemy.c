// src/enemy.c
#include "enemy.h"
#include "sprite.h"

void init_enemies(Enemy enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
}

void spawn_enemy(Enemy enemies[], int river_center_x, int river_width, Sprite* sprite) {
    if (rand() % 100 < 5) { // 5% de chance de spawn
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) {
                enemies[i].active = 1;
                enemies[i].sprite = sprite;
                enemies[i].x = (river_center_x - river_width / 2) + rand() % (river_width - sprite->width);
                enemies[i].y = -sprite->height; // Começa fora da tela
                return;
            }
        }
    }
}

void update_enemies(Enemy enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].y += SCROLL_SPEED;
            if (enemies[i].y > SCREEN_HEIGHT) {
                enemies[i].active = 0;
            }
        }
    }
}

void draw_enemies(Enemy enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            draw_sprite(enemies[i].x, enemies[i].y, enemies[i].sprite);
        }
    }
}