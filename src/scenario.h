// src/scenario.h
#ifndef SCENARIO_H
#define SCENARIO_H

#include "defines.h"

typedef struct {
    int center_x;
    int width;
} River;

void init_walls(Wall walls[]);
void update_scenario(Wall walls[], River* river);
void draw_scenario(Wall walls[]);

#endif