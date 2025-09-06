// src/scenario.c
#include "scenario.h"
#include "graphics.h"

static void generate_river_segment(Wall walls[], int y, River* river) {
    // Encontra um slot para a parede esquerda
    for (int i = 0; i < MAX_WALLS; i++) {
        if (!walls[i].active) {
            walls[i].active = 1;
            walls[i].x = river->center_x - river->width / 2 - WALL_BLOCK_SIZE;
            walls[i].y = y;
            break;
        }
    }
    // Encontra um slot para a parede direita
    for (int i = 0; i < MAX_WALLS; i++) {
        if (!walls[i].active) {
            walls[i].active = 1;
            walls[i].x = river->center_x + river->width / 2;
            walls[i].y = y;
            break;
        }
    }
}

void init_walls(Wall walls[]) {
    for (int i = 0; i < MAX_WALLS; i++) {
        walls[i].active = 0;
    }
}

void first_generate_river(Wall walls[], River* river) {
    for (int y = -WALL_BLOCK_SIZE; y < SCREEN_HEIGHT; y += WALL_BLOCK_SIZE) {
        generate_river_segment(walls, y, river);
    }
}

void update_scenario(Wall walls[], River* river) {
    // Movimenta paredes
    for (int i = 0; i < MAX_WALLS; i++) {
        if (walls[i].active) {
            walls[i].y += SCROLL_SPEED;
            if (walls[i].y > SCREEN_HEIGHT) {
                walls[i].active = 0;
            }
        }
    }

    // Gera novos segmentos do rio e muda sua forma
    static int scroll_counter = 0;
    scroll_counter += SCROLL_SPEED;
    if (scroll_counter >= WALL_BLOCK_SIZE) {
        scroll_counter = 0;
        
        // Altera a largura e o centro do rio
        if (rand() % 10 < 3) river->width += (rand() % 2 == 0 ? 5 : -5);
        if (rand() % 10 < 2) river->center_x += (rand() % 2 == 0 ? 5 : -5);

        // Limites
        if (river->width < 80) river->width = 80;
        if (river->width > 250) river->width = 250;
        if (river->center_x < 100) river->center_x = 100;
        if (river->center_x > SCREEN_WIDTH - 100) river->center_x = SCREEN_WIDTH - 100;

        generate_river_segment(walls, 0, river);
    }
}


void draw_scenario(Wall walls[]) {
    for (int i = 0; i < MAX_WALLS; i++) {
        if (walls[i].active) {
            draw_rect(walls[i].x, walls[i].y, WALL_BLOCK_SIZE, WALL_BLOCK_SIZE, 0xFF008000);
        }
    }
}