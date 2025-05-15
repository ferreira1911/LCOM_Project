#include "menu.h"
#include "state.h"
#include "devices/keyboard.h"
#include "devices/video.h"

#define BUTTON_X 300
#define BUTTON_Y 250
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 100
#define BUTTON_COLOR 0xAAAAAA

int menu_loop() {
    vg_clear_screen();

    // Desenha o botão ou zona do menu
    vg_draw_rectangle(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR);

    // Podes também escrever uma mensagem com vg_draw_text() se tiveres essa função

    // Subscrever interrupção do teclado
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

                if (scancode_ready) {
                    if (scancode == 0x39) {  // 0x39 = makecode da tecla SPACE
                        start_game = true;
                        game_state = PLAYING;
                    }

                    scancode_ready = false; // reset da flag
                }
            }
        }
    }

    kbd_unsubscribe_int();
    return 0;
}
