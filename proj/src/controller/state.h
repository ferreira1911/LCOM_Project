#ifndef STATE_H
#define STATE_H

#include <lcom/lcf.h>

typedef enum {
    MENU,       ///< Estado inicial, mostra o menu
    PLAYING,    ///< Estado durante o jogo
    GAME_OVER   ///< Estado após o fim do jogo
} GameState;

extern GameState game_state;

#endif
