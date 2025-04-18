
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "mouse.h"


int hook_id = 0;
uint8_t mouse_byte;

int (util_sys_outb)(int port, uint8_t value) {
  
    if(sys_outb(port, value) != 0) return 1;
    
    return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
    hook_id = *bit_no;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
        return 1;
    return 0;
}

int mouse_unsubscribe_int() {
    if (sys_irqrmpolicy(&hook_id) != OK)
        return 1;
    return 0;
}

int write_mouse_cmd(uint8_t cmd) {

    uint8_t response;

    for (int retries = 3; retries > 0; retries--) {
        // Step 1: Write 0xD4 to port 0x64
        if (util_sys_outb(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE) != OK) return 1;
        // Step 2: Write command to 0x60
        if (util_sys_outb(KBC_IN_BUF, cmd) != OK) return 1;
        // Step 3: Read ack byte from 0x60
        if (util_sys_inb(KBC_OUT_BUF, &response) != OK) return 1;

        if (response == ACK) return 0;           // 0xFA: sucesso
        else if (response == NACK) continue;     // 0xFE: tenta outra vez
        else if (response == ERROR) return 1;    // 0xFC: erro fatal
    }
    return 1; // se tentou 3 vezes e falhou
}

int enable_mouse_data_reporting() {
    if (write_mouse_cmd(ENABLE_DATA_REPORTING) != OK) 
        return 1;
    return 0;
}

int disable_mouse_data_reporting() {
  if (write_mouse_cmd(DISABLE_DATA_REPORTING) != OK)
    return 1;
  return 0;
}

void (mouse_ih)() {

    uint8_t status;

    if (util_sys_inb(KBC_ST_REG, &status) != OK) return;

    // Check if output buffer is full and no errors
    if ((status & (OBF | AUX)) && !(status & (PAR_ERR | TO_ERR))) {
        util_sys_inb(KBC_OUT_BUF, &mouse_byte);
    }
}

void parse_packet(uint8_t bytes[3], struct package *pp) {

    pp->bytes[0] = bytes[0];
    pp->bytes[1] = bytes[1];
    pp->bytes[2] = bytes[2];

    pp->lb = bytes[0] & BIT(0); //extrai os botões do rato
    pp->rb = bytes[0] & BIT(1);
    pp->mb = bytes[0] & BIT(2); 

    pp->x_ov = bytes[0] & BIT(6); //extrai os overflow
    pp->y_ov = bytes[0] & BIT(7);

    if (bytes[0] & BIT(4)) //o bit 4 indica se o valor de delta x é negativo ou positivo
        pp->delta_x = (int16_t)(0xFF00 | bytes[1]); //faz o signal extension se for negativo
    else {
        pp->delta_x = bytes[1];
    }
    if (bytes[0] & BIT(5)) //o bit 5 indica se o valor de delta y é negativo ou positivo
        pp->delta_y = (int16_t)(0xFF00 | bytes[2]); //faz o signal extension se for negativo
    else {
        pp->delta_y = bytes[2];
    }

}

void print_packet(struct package *pp) {

    printf("B1=0x%02X B2=0x%02X B3=0x%02X ", pp->bytes[0], pp->bytes[1], pp->bytes[2]);
    printf("LB=%d MB=%d RB=%d XOV=%d YOV=%d ", pp->lb, pp->mb, pp->rb, pp->x_ov, pp->y_ov);
    printf("X=%d Y=%d\n", pp->delta_x, pp->delta_y);

}

