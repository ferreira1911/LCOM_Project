#include <lcom/lab3.h>
#include <lcom/lcf.h>

#include "mouse.h"

uint8_t byte_index = 0;       
uint8_t package[3];            
uint8_t current_byte;
int mouse_hookid = 2;

int utils_Write_KBC_Command(uint8_t port, uint8_t commandByte) {
    
    uint8_t status;
    
    for (int attempt = 0; attempt < 10; attempt++) {
        if (read_KBC_status(&status) != 0) {
            return 1;
        }

        if (!(status & BIT(1))) { // buffer de entrada livre
            if (sys_outb(port, commandByte) == 0)
                return 0; // sucesso
            else {
                return 1;
            }
        }
        tickdelay(micros_to_ticks(20000));
    }
    return 1;
}

int utils_Read_KBC_Output(uint8_t port, uint8_t *output) {
    
    uint8_t status;

    for (int attempt = 0; attempt < 10; attempt++) {
        if (read_KBC_status(&status) != 0) {
            return 1;
        }

        if (status & BIT(0)) { // buffer de saída cheio
            if (util_sys_inb(port, output) != 0) {
                return 1;
            }
            if((status & BIT(7)) != 0){                     // verificação de erro de paridade
                return 1;
            }
            if((status & BIT(6)) != 0){                     // verificação de erro de timeout
                return 1;
            }
            if (mouse && !(status & BIT(5))) {              // está à espera do output do rato
                printf("Error: Mouse output not found\n");  // mas o output não é do rato
                return 1;
            } 
            if (!mouse && (status & BIT(5))) {                // está à espera do output do teclado
                printf("Error: Keyboard output not found\n"); // mas o output não é do teclado
                return 1;
            return 0; // sucesso
            }
        tickdelay(micros_to_ticks(20000));
        }
        return 1;
    }
}

int read_KBC_status(uint8_t *status){
    if (status == NULL) {
        return 1; // Erro: ponteiro inválido
    }
    uint8_t output;
    if (utils_Read_KBC_Output(STAT_REG, &output) != 0) {
        return 1; // Erro ao ler o registo de estado
    }
    *status = output; // Armazena o estado lido no ponteiro fornecido
    return 0; // Sucesso
}

void mouse_sync_bytes() {
    // Verifica se é o primeiro byte do pacote (byte de controlo)
    if (byte_index == 0) {
        if (current_byte & BIT(3)) { // Bit 3 ativo indica que é o byte de controlo
            mouse_bytes[byte_index] = current_byte;
            byte_index++;
        }
        // Caso contrário, ignora o byte, pois não é válido como byte de controlo
    }
    else if (byte_index > 0 && byte_index < 3) { // Verifica se está a processar os bytes intermediários
        mouse_bytes[byte_index] = current_byte;
        byte_index++;
    }
    if (byte_index == 3) { // Quando o pacote está completo (3 bytes)
        process_mouse_packet(mouse_bytes); // Processa o pacote completo
        byte_index = 0; // Reinicia o índice para o próximo pacote
    }
}

int write_to_mouse(uint8_t command) {
    uint8_t attempts = 10; // Número máximo de tentativas
    uint8_t mouse_response;

    // Tenta enviar o comando ao rato até obter uma resposta válida ou esgotar as tentativas
    while (attempts > 0) {
        attempts--;

        // Envia o comando para ativar o modo de escrita no rato (0xD4)
        if (write_KBC_command(STAT_REG, 0xD4) != 0) {
            return 1; // Erro ao enviar o comando para o KBC
        }

        // Envia o comando específico para o rato na porta 0x60
        if (write_KBC_command(OUT_BUF, command) != 0) {
            return 1; // Erro ao enviar o comando para o rato
        }

        // Aguarda um curto intervalo para permitir que o rato processe o comando
        tickdelay(micros_to_ticks(20000));

        // Lê a resposta do rato
        if (util_sys_inb(OUT_BUF, &mouse_response) != 0) {
            return 1; // Erro ao ler a resposta do rato
        }

        // Verifica se a resposta do rato é ACK (0xFA)
        if (mouse_response == ACK) {
            return 0; // Sucesso: comando reconhecido pelo rato
        }
    }
    // Se todas as tentativas falharem, retorna erro
    return 1;
}

int mouse_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) {
        printf("Error: Invalid pointer for bit_no\n");
        return 1; // Erro: ponteiro inválido
    }

    *bit_no = BIT(mouse_hookid); // Define a máscara de bits para interrupções do rato

    // Subscrição das interrupções em modo exclusivo
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid) != OK) {
        printf("Error: Failed to subscribe mouse interrupts\n");
        return 1; // Erro ao subscrever interrupções
    }

    return 0; // Sucesso
}

int mouse_unsubscribe_int() {
    // Cancela a subscrição das interrupções
    if (sys_irqrmpolicy(&mouse_hookid) != OK) {
        printf("Error: Failed to unsubscribe mouse interrupts\n");
        return 1; // Erro ao cancelar subscrição
    }

    return 0; // Sucesso
}

void mouse_ih() {
    read_KBC_output(STAT_REG, &current_byte, 1);
  }

