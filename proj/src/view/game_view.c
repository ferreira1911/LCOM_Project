#include <lcom/lcf.h>

#include "game_view.h"
#include "devices/video.h"
#include "numbers.h"

#include "xpms/logo.xpm"
#include "xpms/timer.xpm"
#include "xpms/hits.xpm"
#include "xpms/accuracy.xpm"
#include "xpms/numbers/percentage.xpm"
#include "xpms/fail.xpm"

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

void (draw_game_over_screen)(uint8_t mouse_clicks, uint8_t target_hits, uint8_t seconds){
    vg_clear_screen();

    xpm_image_t logo_img;
    logo_img.bytes = xpm_load((xpm_map_t) logo, XPM_8_8_8, &logo_img);
    vg_draw_xpm(125, 100, &logo_img);

    xpm_image_t timer_img;
    timer_img.bytes = xpm_load((xpm_map_t) timer, XPM_8_8_8, &timer_img);
    vg_draw_xpm(250, 300, &timer_img);

    xpm_image_t hits_img;
    hits_img.bytes = xpm_load((xpm_map_t) hits, XPM_8_8_8, &hits_img);
    vg_draw_xpm(250, 330, &hits_img);

    xpm_image_t accuracy_img;
    accuracy_img.bytes = xpm_load((xpm_map_t) accuracy, XPM_8_8_8, &accuracy_img);
    vg_draw_xpm(250, 360, &accuracy_img);

    uint8_t accuracy_value = 0;
    if (mouse_clicks > 0) {
        accuracy_value = (uint8_t) ((((float)target_hits / mouse_clicks) * 100) + 0.5f);
    }

    draw_number(seconds, 355, 300);
    draw_number(target_hits, 340, 330);
    draw_number(accuracy_value, 400, 360);

    xpm_image_t percentage_img;
    percentage_img.bytes = xpm_load((xpm_map_t) percentage, XPM_8_8_8, &percentage_img);

    uint16_t per_x = (accuracy_value < 10) ? 460 - 28 : 460;
    vg_draw_xpm(per_x, 360, &percentage_img);
}


void (draw_fails)(uint8_t target_fails){
    xpm_image_t fail_img;
    fail_img.bytes = xpm_load((xpm_map_t) fail, XPM_8_8_8, &fail_img);

    vg_draw_xpm(350, 10, &fail_img);

    draw_number(target_fails, 415, 10);
}
