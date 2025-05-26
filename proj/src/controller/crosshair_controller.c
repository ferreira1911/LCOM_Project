#include <lcom/lcf.h>

#include "crosshair_controller.h"

#include "devices/mouse.h"
#include "model/crosshair.h"
#include "target_controller.h"
#include "model/state.h"

extern Crosshair crosshair;
extern uint8_t target_hits;
uint8_t mouse_clicks = 0;
extern GameState game_state;

void (crosshair_controller_init)() {
    crosshair_init(400, 300);
}

void (crosshair_controller_update)(struct packet *pp) {
    crosshair_update_position(pp->delta_x, pp->delta_y);

    if(pp->lb){
        if(game_state != MENU) {
            mouse_clicks++;
        }
        if(target_controller_check_hit(crosshair.x + crosshair.width / 2, crosshair.y + crosshair.height / 2)) {
            target_hits++;
            if(game_state == PLAYING_MODE_1) {
                target_controller_update_mode1();
            }
            if(game_state == PLAYING_MODE_2) {
                target_controller_update_mode2();
            }
        }
            
    }
    
}

void (crosshair_controller_draw)(){
    draw_crosshair(crosshair.x, crosshair.y, crosshair.width, crosshair.height);
}

void (crosshair_controller_reset)() {
    crosshair_init(400, 300);
}
