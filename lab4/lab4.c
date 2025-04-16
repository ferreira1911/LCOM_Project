// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

typedef enum {
    START,
    UP,
    VERTEX,
    DOWN,
    END
} SystemState;

SystemState state = START;
uint16_t x_len_total = 0;

int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status, r;
    message msg;
    uint8_t bit_no;
    struct packet pp;
    uint8_t bytes[3]; // Array para armazenar os 3 bytes do pacote
    uint8_t byte_index = 0; // Índice para rastrear os bytes do pacote

    // Subscrição das interrupções do rato
    if (mouse_subscribe_int(&bit_no) != 0) {
        printf("Error: Failed to subscribe mouse interrupts\n");
        return 1;
    }

    // Habilitar data reporting no rato
    if (mouse_enable_data_reporting() != 0) {
        printf("Error: Failed to enable data reporting\n");
        mouse_unsubscribe_int();
        return 1;
    }

    // Máscara de interrupção
    int irq_set = BIT(bit_no);

    // Loop principal para processar os pacotes
    uint32_t packets_received = 0;
    while (packets_received < cnt) {
        // Espera por uma interrupção
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("Error: driver_receive failed with: %d\n", r);
            continue;
        }

        // Verifica se a interrupção é relevante
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        // Chama o handler de interrupção
                        mouse_ih();

                        // Armazena o byte lido
                        bytes[byte_index] = current_byte;
                        byte_index++;

                        // Verifica se o pacote está completo (3 bytes)
                        if (byte_index == 3) {
                            // Preenche a estrutura do pacote
                            pp.bytes[0] = bytes[0];
                            pp.bytes[1] = bytes[1];ssh 
                            pp.bytes[2] = bytes[2];
                            pp.lb = bytes[0] & BIT(0);
                            pp.rb = bytes[0] & BIT(1);
                            pp.mb = bytes[0] & BIT(2);
                            pp.x_ov = bytes[0] & BIT(6);
                            pp.y_ov = bytes[0] & BIT(7);
                            pp.delta_x = (bytes[0] & BIT(4)) ? (0xFF00 | bytes[1]) : bytes[1];
                            pp.delta_y = (bytes[0] & BIT(5)) ? (0xFF00 | bytes[2]) : bytes[2];

                            // Imprime o pacote
                            mouse_print_packet(&pp);

                            // Incrementa o contador de pacotes recebidos
                            packets_received++;
                            byte_index = 0; // Reinicia o índice para o próximo pacote
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // Desabilitar data reporting no rato
    if (mouse_disable_data_reporting() != 0) {
        printf("Error: Failed to disable data reporting\n");
    }

    // Cancela a subscrição das interrupções
    if (mouse_unsubscribe_int() != 0) {
        printf("Error: Failed to unsubscribe mouse interrupts\n");
        return 1;
    }

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)() {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}