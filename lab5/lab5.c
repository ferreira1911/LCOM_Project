#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>

#include "video.h"
#include "keyboard.h"
#include "kbc.h"

extern char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

extern unsigned h_res;	        /* Horizontal resolution in pixels */
extern unsigned v_res;	        /* Vertical resolution in pixels */
extern unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

extern uint8_t scancode;
extern int counter;

int main(int argc, char *argv[]) {
  // Define a linguagem das mensagens do LCF
  lcf_set_language("EN-US");

  // Habilita o rastreamento de chamadas de função
  // lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // Habilita a gravação da saída de printf
  // lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // Executa a função desejada (pode ser kbd_test_scan ou outra)
  if (lcf_start(argc, argv))
    return 1;

  // Finaliza a execução do LCF
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay){
    if(vbe_set_mode(mode) != 0) return 1;
    
    tickdelay(micros_to_ticks(delay * 1000000));

    if(vg_exit() != 0) return 1;

    return 0;
}

int (wait_for_esc)() {
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
          }
      }
      tickdelay(micros_to_ticks(KBC_DELAY));
  }

  if (kbd_unsubscribe_int() != 0) return 1;
  return 0;
}


int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  if (frame_buffer_init(mode) != 0) return 1;

  if(vbe_set_mode(mode) != 0) return 1;

  vg_draw_rectangle(x,y,width,height,color);

  if (wait_for_esc() != 0) return 1;

  if(vg_exit() != 0) return 1;

  return 0;
}


int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
  vbe_mode_info_t vmi;

  if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

  if (frame_buffer_init(mode) != 0) return 1;

  if(vbe_set_mode(mode) != 0) return 1;

  uint16_t rect_width = vmi.XResolution / no_rectangles;
  uint16_t rect_height = vmi.YResolution / no_rectangles;

  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
        uint16_t x = col * rect_width;
        uint16_t y = row * rect_height;
        
        uint32_t color;
        if (vmi.BitsPerPixel != 8) {
            color = calculate_direct_color(row, col, first, step, vmi);
        } else {
            color = calculate_indexed_color(row, col, first, step, vmi.BitsPerPixel, no_rectangles);
        }


        vg_draw_rectangle(x, y, rect_width, rect_height, color);
    }
  }

  if (wait_for_esc() != 0) return 1;

  if(vg_exit() != 0) return 1;

  return 0;
}

int (video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (frame_buffer_init(0x105) != 0) return 1;

  if(vbe_set_mode(0x105) != 0) return 1;

  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  img.bytes = colors;

  if(vg_draw_xpm(x,y, &img) != 0) return 1;

  if (wait_for_esc() != 0) return 1;

  if (vg_exit() != 0) return 1;

  return 0;
}

int (video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate){
  if (frame_buffer_init(0x105) != 0) return 1;

  if(vbe_set_mode(0x105) != 0) return 1;

  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &img);
  img.bytes = sprite;

  uint16_t current_x = xi;
  uint16_t current_y = yi;

  int delta_x = xf - xi;

  int16_t frame_counter_negative = 0;

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

  int ticks_per_frame = sys_hz() / fr_rate;
  int ticks_counter = 0;

  while ((current_x != xf || current_y != yf) && !esc_released) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & timer_irq_set) {
        timer_int_handler();
        ticks_counter++;

        if(ticks_counter >= ticks_per_frame){
          ticks_counter = 0;
          
          vg_clear_screen();
          
          vg_draw_xpm(current_x, current_y, &img);

          if (speed >= 0){
            if(delta_x != 0){
              if(abs(xf - current_x) <= speed){
                current_x = xf;
              } else {
                current_x += (xf > xi) ? speed : -speed;
              }
            } else{
              if(abs(yf - current_y) <= speed){
                current_y = yf;
              } else {
                current_y += (yf > yi) ? speed : -speed;
              }
            }
          } else{
            frame_counter_negative++;

            if(frame_counter_negative == abs(speed)){
              frame_counter_negative = 0;

              if(delta_x != 0){
                current_x += (xf > xi) ? 1 : -1;
              } else{
                current_y += (yf > yi) ? 1 : -1;
              }
            }
          }
        }

      } else if (msg.m_notify.interrupts & kbd_irq_set) {

        kbc_ih();

        if (scancode == ESC_BREAKCODE) {
          esc_released = true;
        }
      }
    }
  }

  if (kbd_unsubscribe_int() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;

  if(!esc_released){
    if (wait_for_esc() != 0) return 1;
  }

  if (vg_exit() != 0) return 1;

  return 0;
}

