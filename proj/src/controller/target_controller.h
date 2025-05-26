#ifndef TARGET_CONTROLLER_H
#define TARGET_CONTROLLER_H

#include <lcom/lcf.h>

#include "model/target.h"

#define MAX_ACTIVE_TARGETS 3 /**< @brief Maximum number of active targets on the screen */

void (target_controller_init_mode1)();
void (target_controller_init_mode2)();
void (target_controller_init_mode3)();
void (target_controller_update_mode1)();
void (target_controller_update_mode2)();
void (target_controller_update_mode3)();
void (target_controller_horizontal_update)();
void (target_controller_fall_update)();
void (target_controller_draw)();
void (target_controller_reset)();
bool (target_controller_check_hit)(uint16_t x, uint16_t y);
void generate_random_position_in_game_area(int16_t *x, int16_t *y);
void generate_random_position_in_game_area_mode2(int16_t *x, int16_t *y);
void generate_random_position_in_game_area_mode3(int16_t *x, int16_t *y, int *direction);
bool is_overlapping(Target *a, int16_t x, int16_t y, uint16_t width, uint16_t height);

#endif

