#include <lcom/lcf.h>

#include "target_controller.h"

#include "model/target.h"
#include "view/target_view.h"
#include "devices/video.h"
#include "xpms/target.xpm"


extern unsigned h_res;
extern unsigned v_res;

Target targets[MAX_ACTIVE_TARGETS]; /**< @brief Array of targets */
uint8_t target_hits = 0; /**< @brief Number of targets hit */

void (target_controller_init)() {
    srand(time(NULL)); // Initialize random seed

    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x = rand() % (h_res - 200); // 50 é uma margem de "segurança"
        int16_t y = rand() % (v_res - 200);
        targets[i] = create_target(x, y, (xpm_map_t) target);
    }
}

void (target_controller_update)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) {
            int16_t x = rand() % (h_res - 200);
            int16_t y = rand() % (v_res - 200);
            targets[i] = create_target(x, y, (xpm_map_t) target);
        }
    }
}

void (target_controller_draw)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (targets[i].isVisible) {
            draw_target(&targets[i]);
        }
    }
}

bool (target_controller_check_hit)(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) continue;

        int16_t tx = targets[i].x;
        int16_t ty = targets[i].y;
        int16_t tw = targets[i].width;
        int16_t th = targets[i].height;

        if (x < tx + tw && x + width > tx &&  y < ty + th && y + height > ty) {
            targets[i].isVisible = false;
            return true;
        }
    }
    return false;
}

void (target_controller_reset)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        targets[i].isVisible = false;
    }
}

void (target_controller)() {
    target_controller_init();
  
    target_controller_update();
  
    target_controller_draw();
  
    target_controller_reset();
}



