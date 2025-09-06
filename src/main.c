// src/main.c
#include "defines.h"
#include "graphics.h"
#include "game.h" // Garanta que esta linha inclui "game.h" e não "game.c"

PSP_MODULE_INFO("RiverRaid", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-2048);

// --- Callbacks do PSP ---
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

// A declaração de handle_game_input já está em game.h, não precisa ser repetida aqui.

int main(void) {
    setup_callbacks();
    srand(time(NULL));

    init_graphics();
    init_game();

    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_SELECT) {
            break; // Sai do loop para finalizar
        }

        handle_game_input(&pad);
        update_game();
        draw_game();
        
        swap_buffers();
    }

    shutdown_game();
    sceKernelExitGame();
    return 0;
}