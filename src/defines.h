// src/defines.h
#ifndef DEFINES_H
#define DEFINES_H

// --- Includes Globais ---
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspge.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Defines do Jogo ---
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define VRAM_BUFFER_WIDTH 512

#define PLAYER_SPEED 4

#define MAX_BULLETS 5
#define BULLET_WIDTH 3
#define BULLET_HEIGHT 8
#define BULLET_SPEED 6

#define MAX_ENEMIES 10
#define MAX_WALLS 100
#define WALL_BLOCK_SIZE 10

#define SCROLL_SPEED 2

// --- Structs ---
typedef struct {
    int width;
    int height;
    unsigned int* data;
} Sprite;

typedef struct {
    int x, y;
    Sprite* sprite;
} Player;

typedef struct {
    int x, y;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int active;
    Sprite* sprite;
} Enemy;

typedef struct {
    int x, y;
    int active;
} Wall;

#endif