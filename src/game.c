// src/game.c
#include <pspdebug.h> // Este include está correto e deve permanecer
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "scenario.h"
#include "graphics.h"
#include "sprite.h"

// --- Variáveis Globais do Jogo (privadas a este arquivo) ---
static Player player;
static Bullet bullets[MAX_BULLETS];
static Enemy enemies[MAX_ENEMIES];
static Wall walls[MAX_WALLS];
static River river;
static int score;

// Sprites
static Sprite player_sprite;
static Sprite enemy_sprite;

// Estado do jogo
int game_over;

// Declaração externa de `first_generate_river` para uso único na inicialização
extern void first_generate_river(Wall walls[], River* river);
// Declaração externa da nova função de graphics.h
extern void* get_draw_buffer();

void init_game() {
    // Carrega Sprites
    load_sprite(&player_sprite, "assets/player1.png");
    load_sprite(&enemy_sprite, "assets/inimigo2.png");

    // Inicializa Entidades
    init_player(&player, &player_sprite);
    init_bullets(bullets);
    init_enemies(enemies);
    init_walls(walls);

    // Inicializa Cenário
    river.center_x = SCREEN_WIDTH / 2;
    river.width = 150;
    first_generate_river(walls, &river);
    
    score = 0;
    game_over = 0;

    // Inicializa a tela de debug uma vez
    pspDebugScreenInit();
    // Garante que a cor do texto seja branca e visível
    pspDebugScreenSetTextColor(0xFFFFFFFF); 
}

void check_collisions() {
    // (O conteúdo desta função não muda)
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;
            if (bullets[i].x < enemies[j].x + enemies[j].sprite->width &&
                bullets[i].x + BULLET_WIDTH > enemies[j].x &&
                bullets[i].y < enemies[j].y + enemies[j].sprite->height &&
                bullets[i].y + BULLET_HEIGHT > enemies[j].y) {
                bullets[i].active = 0;
                enemies[j].active = 0;
                score += 100;
            }
        }
    }
    for (int i = 0; i < MAX_WALLS; i++) {
        if (!walls[i].active) continue;
        if (player.x < walls[i].x + WALL_BLOCK_SIZE &&
            player.x + player.sprite->width > walls[i].x &&
            player.y < walls[i].y + WALL_BLOCK_SIZE &&
            player.y + player.sprite->height > walls[i].y) {
            game_over = 1;
        }
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        if (player.x < enemies[i].x + enemies[i].sprite->width &&
            player.x + player.sprite->width > enemies[i].x &&
            player.y < enemies[i].y + enemies[i].sprite->height &&
            player.y + player.sprite->height > enemies[i].y) {
            game_over = 1;
        }
    }
}

void update_game() {
    if (game_over) return;

    update_player_bounds(&player);
    update_bullets(bullets);
    update_enemies(enemies);
    update_scenario(walls, &river);
    
    spawn_enemy(enemies, river.center_x, river.width, &enemy_sprite);

    check_collisions();
}

void draw_game() {
    // Aponta a tela de debug para o buffer de desenho correto ANTES de desenhar qualquer coisa.
    // ESTA É A LINHA QUE FOI CORRIGIDA:
    pspDebugScreenSetBase(get_draw_buffer());

    clear_screen(0xFF70483C); // Cor do rio

    draw_scenario(walls);
    draw_enemies(enemies);
    draw_bullets(bullets);
    draw_player(&player);

    // Agora desenha o texto sobre o jogo
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf("Score: %d", score);

    if (game_over) {
        pspDebugScreenSetXY(25, 15);
        pspDebugScreenPrintf("GAME OVER");
        pspDebugScreenSetXY(20, 17);
        pspDebugScreenPrintf("Press START to restart");
    }
}

void handle_game_input(SceCtrlData* pad) {
    if (game_over) {
        if (pad->Buttons & PSP_CTRL_START) init_game();
    } else {
        handle_player_input(&player, pad);
        if (pad->Buttons & PSP_CTRL_CROSS) fire_bullet(&player, bullets);
    }
}

void shutdown_game() {
    free_sprite(&player_sprite);
    free_sprite(&enemy_sprite);
}