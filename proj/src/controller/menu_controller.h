#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include <lcom/lcf.h>

#include "model/menu.h"

#define NUM_MENU_ITEMS 5


void menu_controller_init();
void menu_controller_draw();

int menu_loop();

#endif
