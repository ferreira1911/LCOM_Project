#include <lcom/lcf.h>

#include "game_view.h"
#include "devices/video.h"
#include "numbers.h"

#include "xpms/logo.xpm"
#include "xpms/timer.xpm"
#include "xpms/hits.xpm"

void (draw_hits)(uint8_t target_hits){
    xpm_image_t hits_img;
    hits_img.bytes = xpm_load((xpm_map_t) hits, XPM_8_8_8, &hits_img);

    vg_draw_xpm(650, 10, &hits_img);

    draw_number(target_hits, 738, 10);
}

void (draw_timer)(uint8_t seconds){
    xpm_image_t timer_img;
    timer_img.bytes = xpm_load((xpm_map_t) timer, XPM_8_8_8, &timer_img);

    vg_draw_xpm(10, 10, &timer_img);

    draw_number(seconds, 115, 10);
}

void (draw_game_over_screen)(uint8_t hits){
    vg_clear_screen();

    xpm_image_t logo_img;
    logo_img.bytes = xpm_load((xpm_map_t) logo, XPM_8_8_8, &logo_img);

    vg_draw_xpm(125, 100, &logo_img);

    draw_number(hits, 300, 300);
}
