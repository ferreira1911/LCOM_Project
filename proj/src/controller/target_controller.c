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

void generate_random_position_in_game_area(int16_t *x, int16_t *y) {
    *x = rand() % 726;                     // 0 a 725 => 726 valores
    *y = 40 + rand() % (541 - 40);         // 40 a 540 => 501 valores
}

bool is_overlapping(Target *a, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    return !(x + width  <= a->x || a->x + a->width  <= x || y + height <= a->y || a->y + a->height <= y);
}


void (target_controller_init_mode1)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x, y;
        generate_random_position_in_game_area(&x, &y);
        
        create_target(&targets[i], x, y, (xpm_map_t) target);
    }
}


void (target_controller_init_mode2)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x, y;
        generate_random_position_in_game_area(&x, &y);
        
        create_target(&targets[i], x, y, (xpm_map_t) target);
    }
}


void (target_controller_update_mode1)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) {
            int16_t x, y;
            uint16_t width = targets[i].width;
            uint16_t height = targets[i].height;

            bool valid_position = false;
            int max_attempts = 100;

            while (!valid_position && max_attempts-- > 0) {
                generate_random_position_in_game_area(&x, &y);
                valid_position = true;

                for (int j = 0; j < MAX_ACTIVE_TARGETS; j++) {
                    if (j == i || !targets[j].isVisible) continue;

                    if (is_overlapping(&targets[j], x, y, width, height)) {
                        valid_position = false;
                        break;
                    }
                }
            }
            
            if(valid_position){
                create_target(&targets[i], x, y, (xpm_map_t) target);
            }
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

bool (target_controller_check_hit)(uint16_t x, uint16_t y) {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) continue;

        int16_t tx = targets[i].x;
        int16_t ty = targets[i].y;
        int16_t tw = targets[i].width;
        int16_t th = targets[i].height;

        // Check if the crosshair is within the target's area
        if (x >= tx && x < tx + tw && y >= ty && y < ty + th) {
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



