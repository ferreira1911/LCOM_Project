#include <lcom/lcf.h>

#include "game_view.h"
#include "devices/video.h"
#include "numbers.h"

#include "xpms/logo.xpm"

void (draw_game_over_screen)(uint8_t hits){
    vg_clear_screen();

    xpm_image_t logo_img;
    logo_img.bytes = xpm_load((xpm_map_t) logo, XPM_8_8_8, &logo_img);

    vg_draw_xpm(125, 100, &logo_img);

    draw_number(hits, 300, 300);
}
