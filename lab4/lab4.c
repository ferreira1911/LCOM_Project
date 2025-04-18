#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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

uint8_t packet[3]; // Array to store the mouse packet
extern uint8_t mouse_byte; // Variable to store the mouse byte
uint8_t byte_index = 0;
struct package pp;

int (mouse_test_packet)(uint32_t cnt) {
  
  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no = 2;
  uint32_t packets_read = 0;

  byte_index = 0;

  if (enable_mouse_data_reporting() != OK) return 1;
  if (mouse_subscribe_int(&bit_no) != OK) return 1;

  while (packets_read < cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d\n", r);
      continue;
    }
    
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
              if (msg.m_notify.interrupts & BIT(hook_id)) {
                  mouse_ih(); // Lê o byte do rato → guarda em mouse_byte
                
                  // Verifica se é o 1º byte do pacote (bit 3 tem de estar a 1)
                  if (byte_index == 0 && !(mouse_byte & BIT(3)))
                    break; // Ainda não estamos sincronizados

                  packet[byte_index] = mouse_byte;
                  byte_index++;

                  if (byte_index == 3) {
                    parse_packet(packet, &pp);
                    print_packet(&pp);
                    byte_index = 0;
                    packets_read++;
                  }
                }
                break;
              }
        }   
  }
  if (mouse_unsubscribe_int() != OK) return 1;
  if (disable_mouse_data_reporting() != OK) return 1;

  return 0;
}



