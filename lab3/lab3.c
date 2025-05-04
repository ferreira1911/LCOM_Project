#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdint.h>

#include "keyboard.h"
#include "kbc.h"
#include "i8254.h"

extern uint8_t scancode;
extern int counter;

int main(int argc, char *argv[]) {
    // Define a linguagem das mensagens do LCF
    lcf_set_language("EN-US");
  
    // Habilita o rastreamento de chamadas de função
    /* lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");
    lcf_log_output("/home/lcom/labs/lab3/output.txt"); */
  
    // Executa a função desejada (pode ser kbd_test_scan ou outra)
    if (lcf_start(argc, argv))
      return 1;
  
    // Finaliza a execução do LCF
    lcf_cleanup();
  
    return 0;
  }

int (kbd_test_scan)() {
    uint8_t bit_no = 1;
    int ipc_status;              
    message msg;

    if (kbd_subscribe_int(&bit_no) != 0) return 1;

    int irq_set = BIT(bit_no);
    bool esc_released = false;

    while (!esc_released) {

        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {

            if (msg.m_notify.interrupts & irq_set) {
                
                kbc_ih();

                if (scancode == ESC_BREAKCODE) {
                    esc_released = true;
                }
                if(scancode & BREAKCODE_BIT){
                    kbd_print_scancode(false, 1, &scancode);
                }
                else {
                    kbd_print_scancode(true, 1, &scancode);
                } 
            }
        }
        tickdelay(micros_to_ticks(KBC_DELAY));
    }

    if (kbd_unsubscribe_int() != 0) return 1;
    return 0;
}

int (kbd_test_poll)() {
    bool esc_released = false;

    while (!esc_released) {
        uint8_t status;

        if (util_sys_inb(KBC_STATUS_REGISTER, &status) != 0) return 1;

        if (status & KBC_STATUS_OB_FULL) {
            uint8_t scancode_temp;
            
            if (util_sys_inb(KBC_OUTPUT_BUFFER, &scancode_temp) != 0) {
                return 1;
            } else {
                scancode = scancode_temp;
                if (scancode == ESC_BREAKCODE) {  
                    esc_released = true;
                } 
                if(scancode & BREAKCODE_BIT) {
                    kbd_print_scancode(false, 1, &scancode);
                } 
                else {
                    kbd_print_scancode(true, 1, &scancode);
                }
            }
        }
    }

    uint8_t command_byte;
    if(write_kbc_cmd(KBC_READ_CMD, KBC_COMMAND_REGISTER) != 0) return 1;

    if(read_kbc_cmd(&command_byte) != 0) return 1;

    command_byte = command_byte | KBC_CB_EN_KBD_INT;

    if(write_kbc_cmd(KBC_WRITE_CMD, KBC_COMMAND_REGISTER) != 0) return 1;

    if(write_kbc_cmd(command_byte, KBC_INPUT_BUFFER) != 0) return 1;

    return 0;
}

int (kbd_test_timed_scan)(uint8_t idle) {
    uint8_t kbd_bit_no = 1;
    uint8_t timer_bit_no = 0;
    int ipc_status;              
    message msg;

    if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
    int kbd_irq_set = BIT(kbd_bit_no);

    if (timer_subscribe_int(&timer_bit_no) != 0) return 1;
    int timer_irq_set = BIT(timer_bit_no);

    bool esc_released = false;
    int r;

    uint32_t max_inactive_ticks = idle * 60;
    uint32_t inactive_ticks = 0;

    while(!esc_released && inactive_ticks < max_inactive_ticks){
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & timer_irq_set) {
                
                timer_int_handler();
                inactive_ticks++;

            } else if (msg.m_notify.interrupts & kbd_irq_set) {

                kbc_ih();
                inactive_ticks = 0;

                if (scancode == ESC_BREAKCODE) {
                    esc_released = true;
                }
                if(scancode & BREAKCODE_BIT){
                    kbd_print_scancode(false, 1, &scancode);
                }
                else {
                    kbd_print_scancode(true, 1, &scancode);
                } 
            }
        }
    }


    if (kbd_unsubscribe_int() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;
    return 0;
}
