// src/player.c
#include "player.h"
#include "graphics.h" // para draw_rect
#include "sprite.h"   // para draw_sprite

void init_player(Player* player, Sprite* sprite) {
    player->x = SCREEN_WIDTH / 2 - sprite->width / 2;
    player->y = SCREEN_HEIGHT - sprite->height - 10;
    player->sprite = sprite;
}

void handle_player_input(Player* player, SceCtrlData* pad) {
    if (pad->Buttons & PSP_CTRL_LEFT) player->x -= PLAYER_SPEED;
    if (pad->Buttons & PSP_CTRL_RIGHT) player->x += PLAYER_SPEED;
}

void update_player_bounds(Player* player) {
    if (player->x < 0) player->x = 0;
    if (player->x > SCREEN_WIDTH - player->sprite->width) {
        player->x = SCREEN_WIDTH - player->sprite->width;
    }
}

void draw_player(Player* player) {
    draw_sprite(player->x, player->y, player->sprite);
}

// Lógica das Balas
void init_bullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

void fire_bullet(Player* player, Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].x = player->x + player->sprite->width / 2 - BULLET_WIDTH / 2;
            bullets[i].y = player->y;
            return;
        }
    }
}

void update_bullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_SPEED;
            if (bullets[i].y < 0) {
                bullets[i].active = 0;
            }
        }
    }
}

void draw_bullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            draw_rect(bullets[i].x, bullets[i].y, BULLET_WIDTH, BULLET_HEIGHT, 0xFF0000FF);
        }
    }
}