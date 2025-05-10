#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <lcom/lcf.h>

typedef struct {
    int x, y;
    int width, height;
} Crosshair;

void crosshair_init(int x, int y);
void crosshair_update_position(int delta_x, int delta_y);

#endif

