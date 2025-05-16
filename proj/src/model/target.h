#ifndef TARGET_H
#define TARGET_H

#include <lcom/lcf.h>

typedef struct {
    int16_t x, y;
    uint16_t width, height;
    xpm_image_t TargetImage;
    bool isVisible;
} Target;

void (create_target)(Target* target, int16_t x, int16_t y, xpm_map_t image);
void (target_update_position)(Target *target, int16_t delta_x, int16_t delta_y);

#endif

