#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <lcom/lcf.h>

#include "devices/mouse.h"
#include "model/crosshair.h"
#include "view/crosshair_view.h"


void (crosshair_controller_init)();
void (crosshair_controller_update)(struct packet *pp);
void (crosshair_controller_draw)();
void (crosshair_controller_reset)();

#endif

