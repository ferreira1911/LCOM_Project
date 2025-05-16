#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <lcom/lcf.h>

#define VBE_GAME_MODE 0x115 /**< @brief 800x600 - Direct Color - 24 (8:8:8) */
#define GAME_MODE_1_DURATION 30 /**< @brief Duration of game mode 1 in seconds */


int (game_controller)();
int (game_init)();
int (game_loop)();
int (game_exit)();
int (draw_game_elements)();

#endif
