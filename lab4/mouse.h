#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <stdbool.h>

#define MOUSE_IRQ 12

#define KBC_CMD_REG 0x64
#define KBC_IN_BUF  0x60
#define KBC_OUT_BUF 0x60
#define KBC_ST_REG  0x64

#define WRITE_BYTE_TO_MOUSE 0xD4

#define ACK  0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define OBF  BIT(0)
#define AUX  BIT(5)
#define TO_ERR  BIT(6)
#define PAR_ERR BIT(7)

#define ENABLE_DATA_REPORTING  0xF4
#define DISABLE_DATA_REPORTING 0xF5

extern int hook_id; // Variable to store the hook id

struct package {
    uint8_t bytes[3];
    bool lb, rb, mb;
    int16_t delta_x, delta_y;
    bool x_ov, y_ov;
};

int (util_sys_outb)(int port, uint8_t value);
int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();

int enable_mouse_data_reporting();
int disable_mouse_data_reporting();


void (mouse_ih)();

int write_mouse_cmd(uint8_t cmd);

extern uint8_t mouse_byte;

void parse_packet(uint8_t bytes[3], struct package *pp);
void print_packet(struct package *pp);


#endif 

