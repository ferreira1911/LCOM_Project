#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <lcom/lcf.h>

typedef struct {
    int16_t x, y;
    uint16_t width, height;
} Crosshair;

void (crosshair_init)(int16_t x, int16_t y);
void (crosshair_update_position)(int16_t delta_x, int16_t delta_y);

#endif

