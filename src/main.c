
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspge.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

PSP_MODULE_INFO("RiverRaid", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-2048);

// --- Defines ---
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
} Player;

typedef struct {
    int x, y;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int active;
} Enemy;

typedef struct {
    int x, y;
    int active;
} Wall;

// --- Globals ---
Player player;
Bullet bullets[MAX_BULLETS];
Enemy enemies[MAX_ENEMIES];
Wall walls[MAX_WALLS];
int score;
int game_over;

Sprite player_sprite;
Sprite enemy_sprite;

int river_center_x;
int river_width;

// --- Sprite Functions ---
void load_sprite(const char* filename, Sprite* sprite) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
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
}

void draw_sprite(int sx, int sy, Sprite* sprite) {
    if (!sprite->data) return;
    unsigned int* vram = (unsigned int*)sceGeEdramGetAddr() + sx + sy * VRAM_BUFFER_WIDTH;
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

// --- Graphics ---
void draw_rect(int x, int y, int w, int h, unsigned int color) {
    unsigned int* vram = (unsigned int*)sceGeEdramGetAddr() + x + y * VRAM_BUFFER_WIDTH;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            vram[i] = color;
        }
        vram += VRAM_BUFFER_WIDTH;
    }
}

void clear_screen(unsigned int color) {
    unsigned int* vram = (unsigned int*)sceGeEdramGetAddr();
    for (int i = 0; i < VRAM_BUFFER_WIDTH * SCREEN_HEIGHT; i++) {
        vram[i] = color;
    }
}

// --- Game Logic ---
void fire_bullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].x = player.x + player_sprite.width / 2 - BULLET_WIDTH / 2;
            bullets[i].y = player.y;
            return;
        }
    }
}

void generate_river_segment(int y) {
    for (int i = 0; i < MAX_WALLS; i++) {
        if (!walls[i].active) {
            walls[i].active = 1;
            walls[i].x = river_center_x - river_width / 2 - WALL_BLOCK_SIZE;
            walls[i].y = y;
            break;
        }
    }
     for (int i = 0; i < MAX_WALLS; i++) {
        if (!walls[i].active) {
            walls[i].active = 1;
            walls[i].x = river_center_x + river_width / 2;
            walls[i].y = y;
            break;
        }
    }
}

void spawn_enemy(int y) {
    if (rand() % 100 < 5) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) {
                enemies[i].active = 1;
                enemies[i].x = (river_center_x - river_width/2) + rand() % (river_width - enemy_sprite.width);
                enemies[i].y = y;
                return;
            }
        }
    }
}

void init_game() {
    player.x = SCREEN_WIDTH / 2 - player_sprite.width / 2;
    player.y = SCREEN_HEIGHT - player_sprite.height - 10;

    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].active = 0;
    for (int i = 0; i < MAX_WALLS; i++) walls[i].active = 0;

    river_center_x = SCREEN_WIDTH / 2;
    river_width = 150;

    for (int y = 0; y < SCREEN_HEIGHT; y += WALL_BLOCK_SIZE) {
        generate_river_segment(y);
    }

    score = 0;
    game_over = 0;
}

void update_game() {
    if (game_over) return;

    for (int i = 0; i < MAX_WALLS; i++) {
        if (walls[i].active) {
            walls[i].y += SCROLL_SPEED;
            if (walls[i].y > SCREEN_HEIGHT) walls[i].active = 0;
        }
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].y += SCROLL_SPEED;
            if (enemies[i].y > SCREEN_HEIGHT) enemies[i].active = 0;
        }
    }

    static int scroll_counter = 0;
    scroll_counter += SCROLL_SPEED;
    if (scroll_counter >= WALL_BLOCK_SIZE) {
        scroll_counter = 0;
        if (rand() % 10 < 3) river_width += (rand() % 2 == 0 ? 5 : -5);
        if (rand() % 10 < 2) river_center_x += (rand() % 2 == 0 ? 5 : -5);
        if (river_width < 80) river_width = 80;
        if (river_width > 250) river_width = 250;
        if (river_center_x < 100) river_center_x = 100;
        if (river_center_x > SCREEN_WIDTH - 100) river_center_x = SCREEN_WIDTH - 100;

        generate_river_segment(0);
        spawn_enemy(0);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_SPEED;
            if (bullets[i].y < 0) bullets[i].active = 0;
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;
            if (bullets[i].x < enemies[j].x + enemy_sprite.width &&
                bullets[i].x + BULLET_WIDTH > enemies[j].x &&
                bullets[i].y < enemies[j].y + enemy_sprite.height &&
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
            player.x + player_sprite.width > walls[i].x &&
            player.y < walls[i].y + WALL_BLOCK_SIZE &&
            player.y + player_sprite.height > walls[i].y) {
            game_over = 1;
        }
    }
     for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        if (player.x < enemies[i].x + enemy_sprite.width &&
            player.x + player_sprite.width > enemies[i].x &&
            player.y < enemies[i].y + enemy_sprite.height &&
            player.y + player_sprite.height > enemies[i].y) {
            game_over = 1;
        }
    }
}

void draw() {
    clear_screen(0xFF70483C);

    for (int i = 0; i < MAX_WALLS; i++) {
        if(walls[i].active) draw_rect(walls[i].x, walls[i].y, WALL_BLOCK_SIZE, WALL_BLOCK_SIZE, 0xFF008000);
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if(enemies[i].active) draw_sprite(enemies[i].x, enemies[i].y, &enemy_sprite);
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) draw_rect(bullets[i].x, bullets[i].y, BULLET_WIDTH, BULLET_HEIGHT, 0xFF0000FF);
    }
    draw_sprite(player.x, player.y, &player_sprite);

    pspDebugScreenInit();
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf("Score: %d", score);

    if (game_over) {
        pspDebugScreenSetXY(25, 15);
        pspDebugScreenPrintf("GAME OVER");
        pspDebugScreenSetXY(20, 17);
        pspDebugScreenPrintf("Press START to restart");
    }
}

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

void setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }
}

int main(void) {
    setup_callbacks();
    srand(time(NULL));

    sceDisplaySetMode(0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceDisplaySetFrameBuf(sceGeEdramGetAddr(), VRAM_BUFFER_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_IMMEDIATE);

    load_sprite("assets/player1.png", &player_sprite);
    load_sprite("assets/inimigo2.png", &enemy_sprite);

    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    init_game();

    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (game_over) {
            if (pad.Buttons & PSP_CTRL_START) init_game();
        } else {
            if (pad.Buttons & PSP_CTRL_LEFT) player.x -= PLAYER_SPEED;
            if (pad.Buttons & PSP_CTRL_RIGHT) player.x += PLAYER_SPEED;
            if (pad.Buttons & PSP_CTRL_CROSS) fire_bullet();

            if (player.x < 0) player.x = 0;
            if (player.x > SCREEN_WIDTH - player_sprite.width) player.x = SCREEN_WIDTH - player_sprite.width;
        }

        if (pad.Buttons & PSP_CTRL_SELECT) sceKernelExitGame();

        update_game();
        draw();
        sceDisplayWaitVblankStart();
    }

    sceKernelExitGame();
    return 0;
}
