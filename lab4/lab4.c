#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "mouse.h"

extern uint8_t byte_index;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  // lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
  
  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no = 2;

  if (mouse_subscribe_int(&bit_no) != 0) return 1;
  int irq_set = BIT(bit_no);

  if (write_mouse_cmd(MOUSE_DATA_REPORT_ENABLE) != 0) return 1;

  while (cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
            sync_bytes();

            if (byte_index == 3) {
              struct packet pp;

              parse_packet(&pp);
              mouse_print_packet(&pp);
              
              byte_index = 0;
              cnt--;
            }

          }
          break;
      }
    }   
  }
  
  if (write_mouse_cmd(MOUSE_DATA_REPORT_DISABLE) != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time){
    
  int ipc_status;
  message msg;

  uint8_t mouse_bit_no = 2;
  uint8_t timer_bit_no = 0;

  if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
  int mouse_irq_set = BIT(mouse_bit_no);

  if (timer_subscribe_int(&timer_bit_no) != 0) return 1;
  int timer_irq_set = BIT(timer_bit_no);

  if (write_mouse_cmd(MOUSE_DATA_REPORT_ENABLE) != 0) return 1;

  uint32_t max_inactive_ticks = idle_time * sys_hz();
  uint32_t inactive_ticks = 0;

  while (inactive_ticks < max_inactive_ticks) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
 
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      
      if (msg.m_notify.interrupts & timer_irq_set) {
        timer_int_handler();
        inactive_ticks++;
      }
      
      if (msg.m_notify.interrupts & mouse_irq_set) {
        mouse_ih();
        sync_bytes();

        if (byte_index == 3) {
          struct packet pp;

          parse_packet(&pp);
          mouse_print_packet(&pp);
          
          byte_index = 0;
        }

        inactive_ticks = 0;
      }
    }
  }   
  
  if (write_mouse_cmd(MOUSE_DATA_REPORT_DISABLE) != 0) return 1;

  if (timer_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){

  return 0;
}



