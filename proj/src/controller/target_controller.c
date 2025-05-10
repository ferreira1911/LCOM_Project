#include <lcom/lcf.h>

#include "target_controller.h"

#include "model/target.h"
#include "view/target_view.h"
#include "devices/video.h"
#include "xpms/target.xpm"


extern unsigned h_res;
extern unsigned v_res;

Target targets[MAX_ACTIVE_TARGETS]; /**< @brief Array of targets */

void (target_controller_init)() {
    srand(time(NULL)); // Initialize random seed

    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x = rand() % (h_res - 50); // 50 é uma margem de "segurança"
        int16_t y = rand() % (v_res - 50);
        targets[i] = create_target(x, y, (xpm_map_t) target);
    }
}

void (target_controller_update)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) {
            int16_t x = rand() % (h_res - 50);
            int16_t y = rand() % (v_res - 50);
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



