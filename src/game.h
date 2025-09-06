// src/game.h
#ifndef _GAME_H_
#define _GAME_H_

#include <psptypes.h>
#include <pspctrl.h>

// --- Funções Públicas do Jogo ---

// Inicializa o estado do jogo
void init_game();

// Atualiza a lógica do jogo (movimento, colisões, etc.)
void update_game();

// Desenha todos os elementos do jogo na tela
void draw_game();

// Libera recursos carregados pelo jogo
void shutdown_game();

// Processa a entrada do controle do jogador
void handle_game_input(SceCtrlData* pad);

#endif