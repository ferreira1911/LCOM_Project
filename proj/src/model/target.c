#include <lcom/lcf.h>

#include "target.h"
#include "devices/video.h"

extern unsigned h_res;
extern unsigned v_res;

Target create_target(int16_t x, int16_t y, xpm_map_t image){
    Target target;
    target.x = x;
    target.y = y;

    target.TargetImage.bytes = xpm_load(image, XPM_8_8_8, &target.TargetImage);
    target.isVisible = true;

    target.width = target.TargetImage.width;
    target.height = target.TargetImage.height;

    return target;
}

void target_update_position(Target *target, int16_t delta_x, int16_t delta_y){
    target->x += delta_x;
    target->y -= delta_y;

    if (target->x < 0){
        target->x = 0;
    } else if (target->x + target->width > (int16_t) h_res) {
        target->x = (int16_t) h_res - target->width;
    }

    if (target->y < 0) {
        target->y = 0;
    }
    else if (target->y + target->height > (int16_t) v_res) {
        target->y = (int16_t) v_res - target->height;
    }
}



