#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

// IRQ Line do teclado ()
#define KBD_IRQ 1

// Registos do i8042
#define KBC_OUT_BUF 0x60
#define KBC_STATUS_REG 0x64

int (kbd_unsubscribe_int)();
int (kbd_subscribe_int)(uint8_t *bit_no);
void (kbc_ih1)(void);

#endif /* KEYBOARD_H */

