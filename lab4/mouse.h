#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>

// IRQ Line do teclado ()
#define MOUSE_IRQ 12
#define STAT_REG 0x64
#define OUT_BUF 0x60

// Registos do i8042


int utils_Read_KBC_Output(uint8_t port, uint8_t *output);
int utils_Write_KBC_Command(uint8_t port, uint8_t commandByte);
int read_KBC_status(uint8_t *status);
void mouse_ih();
void mouse_sync_bytes();
int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();

#endif 

