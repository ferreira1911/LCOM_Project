#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  if(timer != 0) return 1;

  uint8_t readBackCommand = 0xC2; // 11000010 (Ver na Table 2: Format of the i8254 Read-Back command.)
  sys_outb(TIMER_CTRL, readBackCommand);

  if(util_sys_inb(TIMER_0, st) != 0) return 1;
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  /* To be implemented by the students */
  if(timer != 0) return 1;

  switch (field)
  {
  case tsf_all:
    printf("Complete Configuration: 0x%02X\n", st);
    break;
  
  case tsf_initial:
    {
      uint8_t initial = (st >> 4) & 0x03;
      switch (initial)
      {
      case 0x01:
        printf("Initialization Mode: LSB\n");
        break;
      
      case 0x02:
        printf("Initialization Mode: MSB\n");
        break;
      
      case 0x03:
        printf("Initialization Mode: LSB followed by MSB\n");
        break;
      
      default:
        printf("Initialization Mode: Invalid\n");
        break;
      }
    }
    break;
  
  case tsf_mode:
    {
      uint8_t mode = (st >> 1) & 0x07;
      printf("Operating Mode: %d\n", mode);
    }
    break;
  
  case tsf_base:
    {
      uint8_t base = st & 1;
      if(base == 0){
        printf("Counting base: Binary(16 bits)\n");
      }else{
        printf("Counting base: BCD(4 decades)\n");
      }
    }
    break;
  
  default:
    printf("Campo Inválido!\n");
    break;

  }

  return 0;
}
