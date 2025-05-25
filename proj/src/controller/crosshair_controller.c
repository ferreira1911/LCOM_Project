#include <lcom/lcf.h>

#include "crosshair_controller.h"

#include "devices/mouse.h"
#include "model/crosshair.h"
#include "target_controller.h"

extern Crosshair crosshair;
extern uint8_t target_hits;

void (crosshair_controller_init)() {
    crosshair_init(400, 300);
}

void (crosshair_controller_update)(struct packet *pp) {
    crosshair_update_position(pp->delta_x, pp->delta_y);

    if(pp->lb){
        if(target_controller_check_hit(crosshair.x + crosshair.width / 2, crosshair.y + crosshair.height / 2)) {
            target_hits++;
            target_controller_update();
        }
            
    }
    
}

void (crosshair_controller_draw)(){
    draw_crosshair(crosshair.x, crosshair.y, crosshair.width, crosshair.height);
}

void (crosshair_controller_reset)() {
    crosshair_init(400, 300);
}

void (crosshair_controller)(struct packet *pp) {
    crosshair_controller_init();

    crosshair_controller_update(pp);

    crosshair_controller_draw();

    crosshair_controller_reset();
}
