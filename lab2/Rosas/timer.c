#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer != 0)return 1;
  uint8_t st;
  if(timer_get_conf(timer, &st) != 0) return 1;
  uint8_t control = (st & 0x0F);
  control |= TIMER_LSB_MSB;
  if(sys_outb(TIMER_CTRL, control) != 0)return 1;
  uint16_t div = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  if(util_get_LSB(div, &lsb) != 0)return 1;
  if(util_get_MSB(div, &msb) != 0)return 1;
  if(sys_outb(TIMER_0, lsb) != 0)return 1;
  if(sys_outb(TIMER_0, msb) != 0)return 1;
  return 0;
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
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
