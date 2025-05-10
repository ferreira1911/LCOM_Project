#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <lcom/lcf.h>

#define VBE_GAME_MODE 0x115 /**< @brief 800x600 - Direct Color - 24 (8:8:8) */

int (game_controller)();
int (game_init)();
int (game_loop)();
int (game_exit)();

#endif
