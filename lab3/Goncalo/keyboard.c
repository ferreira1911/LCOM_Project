#include <lcom/lab3.h>
#include <lcom/lcf.h>

#include "keyboard.h"

int hook_id = 0;
uint8_t scancode;

int (kbd_subscribe_int)(uint8_t *bit_no) {
    hook_id = *bit_no;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (kbd_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id);
}

void kbd_ih_handler(void) {
    uint8_t status;
    if (sys_inb(KBC_STATUS_REG, (uint32_t *)&status) != OK) {
        printf("Erro ao ler o status do KBC!\n");
        return;
    }

    if (status & 0x01) {
        uint8_t scancode_temp;
        if (sys_inb(KBC_OUT_BUF, (uint32_t *)&scancode_temp) != OK) {
            printf("Erro ao ler o código de escaneamento!\n");
        } else {
            scancode = scancode_temp;
        }
    }
}


