#include <lcom/lcf.h>

#include "game.h"
#include "crosshair_controller.h"
#include "target_controller.h"
#include "model/state.h"
#include "menu_controller.h"
#include "view/game_view.h"

#include "devices/i8254.h"
#include "devices/kbc.h"
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/video.h"

extern uint8_t scancode;
extern uint8_t byte_index;
extern int counter;

extern uint8_t target_hits;
GameState game_state;

int (draw_game_elements)() {
    vg_clear_screen();

    target_controller_draw();

    crosshair_controller_draw();
    
    return 0;
}

int (game_init)() {
    vg_clear_screen();
    crosshair_controller_init();
    crosshair_controller_draw();

    target_controller_init();
    target_controller_draw();

    return 0;
}

int (game_loop)(){
    uint8_t timer_bit_no = 0;
    uint8_t kbd_bit_no = 1;
    uint8_t mouse_bit_no = 2;

    int ipc_status;              
    message msg;

    if (timer_subscribe_int(&timer_bit_no) != 0) return 1;
    int timer_irq_set = BIT(timer_bit_no);

    if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
    int kbd_irq_set = BIT(kbd_bit_no);

    if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    int mouse_irq_set = BIT(mouse_bit_no);

    if (write_mouse_cmd(MOUSE_DATA_REPORT_ENABLE) != 0) return 1;

    bool esc_released = false;
    uint8_t seconds_counter = 0;

    while (!esc_released) {

        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
  
        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
  
            if (msg.m_notify.interrupts & kbd_irq_set) {
                
                kbc_ih();
  
                if (scancode == ESC_BREAKCODE) {
                    esc_released = true;
                    break;
                }
            }

            if (msg.m_notify.interrupts & mouse_irq_set) {
                mouse_ih();
                sync_bytes();

                if (byte_index == 3) {
                  struct packet pp;
                
                  parse_packet(&pp);
                  crosshair_controller_update(&pp);

                  if(draw_game_elements() != 0) return 1;
                  
                  byte_index = 0;
                }
            }

            if (msg.m_notify.interrupts & timer_irq_set) {
                timer_int_handler();
                
                if (counter % 60 == 0) {
                    seconds_counter++;
                    if (seconds_counter >= GAME_MODE_1_DURATION) {
                        esc_released = true;
                        break;
                    }
                }
            }
        }
    }

    if (timer_unsubscribe_int() != 0) return 1;
    if (kbd_unsubscribe_int() != 0) return 1;
    if (write_mouse_cmd(MOUSE_DATA_REPORT_DISABLE) != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    
    return 0;
}

int (game_exit)(){
    if (vg_exit() != 0) return 1;

    printf("Game Over! You hit %d targets in %d seconds\n", target_hits, GAME_MODE_1_DURATION);

    return 0;
}

int (game_controller)() {
    if (frame_buffer_init(VBE_GAME_MODE) != 0) return 1;
    
    if (vbe_set_mode(VBE_GAME_MODE) != 0) return 1;

    game_state = MENU;

    if(menu_loop() != 0) return 1;

    if (game_state == GAME_OVER) {
        if (vg_exit() != 0) return 1;
        return 0;
    }

    if (game_init() != 0) return 1;

    if (game_loop() != 0) return 1;

    game_state = GAME_OVER;

    draw_game_over_screen(target_hits);

    tickdelay(micros_to_ticks(3000000));

    if (game_exit() != 0) return 1;

    return 0;
}
