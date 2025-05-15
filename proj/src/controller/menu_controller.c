#include <lcom/lcf.h>

#include "menu_controller.h"
#include "state.h"

#include "devices/keyboard.h"
#include "devices/video.h"
#include "view/menu_view.h"

#include "xpms/logo.xpm"
#include "xpms/classic.xpm"
#include "xpms/falling.xpm"
#include "xpms/fly.xpm"
#include "xpms/quit.xpm"

MenuItem menu_items[NUM_MENU_ITEMS];

extern uint8_t scancode;

void menu_controller_init() {
    create_menu_item(&menu_items[0], 250, 40, (xpm_map_t) logo);
    create_menu_item(&menu_items[1], 229, 310, (xpm_map_t) classic);
    create_menu_item(&menu_items[2], 425, 310, (xpm_map_t) falling);
    create_menu_item(&menu_items[3], 229, 414, (xpm_map_t) fly);
    create_menu_item(&menu_items[4], 425, 414, (xpm_map_t) quit);
}


void menu_controller_draw() {
    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        if (menu_items[i].isVisible)
            draw_menu_item(&menu_items[i]);
    }
}

int menu_loop() {
    menu_controller_init();
    vg_clear_screen();
    menu_controller_draw();


    uint8_t kb_bit_no = 1;
    if (kbd_subscribe_int(&kb_bit_no) != 0) return 1;
    int kb_irq_set = BIT(kb_bit_no);

    message msg;
    int ipc_status;
    bool start_game = false;

    while (!start_game) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & kb_irq_set) {
                kbc_ih();  // interrupção do teclado

                if (scancode == 0x02) {  // 0x02 = makecode da tecla 1
                    start_game = true;
                    game_state = PLAYING;
                    break;
                }
            }
        }
    }

    kbd_unsubscribe_int();
    return 0;
}
