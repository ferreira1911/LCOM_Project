#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdint.h>

#include "keyboard.h"

extern uint8_t scancode;

int main(int argc, char *argv[]) {
    // Define a linguagem das mensagens do LCF
    lcf_set_language("EN-US");
  
    // Habilita o rastreamento de chamadas de função
    // lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");
  
    // Habilita a gravação da saída de printf
    // lcf_log_output("/home/lcom/labs/lab3/output.txt");
  
    // Executa a função desejada (pode ser kbd_test_scan ou outra)
    if (lcf_start(argc, argv))
      return 1;
  
    // Finaliza a execução do LCF
    lcf_cleanup();
  
    return 0;
  }

int (kbd_test_scan)() {

    printf("Pressione ESC para sair...\n");
    uint8_t bit_no = 1;
    int ipc_status;
    message msg;

    if (kbd_subscribe_int(&bit_no) != 0) return 1;

    int irq_set = BIT(bit_no);
    bool esc_released = false;

    while (!esc_released) {
        printf("Entrou aqui\n");

        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        printf("Entrou aqui1\n");

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            printf("Entrou aqui2\n");
            if (msg.m_notify.interrupts & irq_set) {
                printf("Entrou aqui3\n");
                kbc_ih();
                if (scancode == 0x81) {
                    esc_released = true;
                    printf("ESC pressionado, a sair...\n");
                }else if(scancode & 0x80){
                    printf("Entrou aqui4\n");
                    kbd_print_scancode(false, 1, &scancode);
                }
                else {
                    printf("Entrou aqui5\n");
                    kbd_print_scancode(true, 1, &scancode);
                }
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) return 1;
    return 0;
}

int (kbd_test_poll)() {

    printf("Pressione ESC para sair...\n");
    bool esc_released = false;

    while (!esc_released) {
        uint8_t status;

        if (sys_inb(KBC_STATUS_REG, (uint32_t *)&status) != OK) {
            printf("Erro ao ler o status do KBC!\n");
            return 1;
        }

        if (status & 0x01) {
            uint8_t scancode_temp;
            
            if (sys_inb(KBC_OUT_BUF, (uint32_t *)&scancode_temp) != OK) {
                printf("Erro ao ler o scancode!\n");
                return 1;
            } else {
                scancode = scancode_temp;
                if (scancode == 0x81) {  
                    esc_released = true;
                    printf("ESC pressionado, a sair...\n");
                } 
                else if(scancode & 0x80) {
                    kbd_print_scancode(false, 1, &scancode);
                } 
                else {
                    kbd_print_scancode(true, 1, &scancode);
                }
            }
        }
    }
    return 0;
}
