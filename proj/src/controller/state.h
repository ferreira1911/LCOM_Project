#ifndef STATE_H
#define STATE_H

#include <lcom/lcf.h>

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

extern GameState game_state;

#endif
