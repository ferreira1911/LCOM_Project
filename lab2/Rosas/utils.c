#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  if(lsb == NULL)return 1;
  *lsb = (val & 0x00FF);
  printf("%s is implemented!\n", __func__);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL)return 1;
  *msb = ((val >> 8) & 0xFF00)
  printf("%s is implemented!\n", __func__);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
  if(value == NULL) return 1; 
  if (sys_inb(port, &temp) != 0) return 1;
  *value = (uint8_t) temp;
  printf("%s is implemented!\n", __func__);

  return  0;
}
