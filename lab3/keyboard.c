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

void (kbc_ih1)(void) {

    uint8_t status;
    
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
        printf("Erro ao ler o status do KBC!\n");
        return;
    }

    if (status & 0x01) {
        uint8_t scancode_temp;
        if (util_sys_inb(KBC_OUT_BUF, &scancode_temp) != 0) {
        } else {
            scancode = scancode_temp;
        }
    }
}


