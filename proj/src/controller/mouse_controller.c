#include <lcom/lcf.h>

#include "mouse_controller.h"

#include "devices/mouse.h"
#include "model/crosshair.h"

void handle_mouse_packet(struct packet *pp) {
    crosshair_update_position(pp->delta_x, pp->delta_y);

    if (pp->lb) {
        printf("Botão esquerdo clicado\n");
    }

    if (pp->rb) {
        printf("Botão direito clicado\n");
    }

    if (pp->mb) {
        printf("Botão do meio clicado\n");
    }
}
