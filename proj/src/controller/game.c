#include <lcom/lcf.h>

#include "devices/i8254.h"
#include "devices/kbc.h"
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/video.h"

#include "game.h"

int (game_controller)() {
    if (frame_buffer_init(VBE_GAME_MODE) != 0) return 1;
    
    if (vbe_set_mode(VBE_GAME_MODE) != 0) return 1;

    tickdelay(micros_to_ticks(10 * 1000000));

    if (vg_exit() != 0) return 1;

    return 0;
}
