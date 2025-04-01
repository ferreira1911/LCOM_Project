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

void (kbc_ih)(void) {
    uint8_t status;
    printf("Entrou aqui6\n");
    if (sys_inb(KBC_STATUS_REG, (uint32_t *)&status) != 0) {
        printf("Erro ao ler o status do KBC!\n");
        return;
    }

    printf("Entrou aqui7\n");
    if (status & 0x01) {
        printf("Entrou aqui8\n");
        uint8_t scancode_temp;
        if (sys_inb(KBC_OUT_BUF, (uint32_t *)&scancode_temp) != 0) {
            printf("Erro ao ler o scancode!\n");
        } else {
            printf("Entrou aqui9\n");
            scancode = scancode_temp;
        }
    }
}


