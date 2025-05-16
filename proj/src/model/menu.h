#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>

typedef struct {
    int16_t x, y;
    uint16_t width, height;
    xpm_image_t ItemImage;
    bool isVisible;
} MenuItem;

void (create_menu_item)(MenuItem* item, int16_t x, int16_t y, xpm_map_t image);

#endif
