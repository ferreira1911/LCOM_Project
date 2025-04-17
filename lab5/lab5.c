#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>

#include "video.h"

extern char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

extern unsigned h_res;	        /* Horizontal resolution in pixels */
extern unsigned v_res;	        /* Vertical resolution in pixels */
extern unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

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
    // Passar para modo gráfico
    if(vbe_set_mode(mode) != 0) return 1;
    

    tickdelay(micros_to_ticks(delay * 1000000));

    // Passar para modo texto
    if(vg_exit() != 0) return 1;
    printf("Modo Texto\n");

    return 0;
}

//Falta ligar ao keyboard (ESC 0x81)
int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  vbe_mode_info_t vmi;

  // Obter informações sobre o modo gráfico
  if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

  // Passar para modo gráfico
  if(vbe_set_mode(mode) != 0) return 1;

  // Mapear a VRAM (frame buffer) para o processo
  if (frame_buffer_init(mode) != 0) return 1;

  // Desenhar o retângulo no frame buffer
  vg_draw_rectangle(x,y,width,height,color);

  tickdelay(micros_to_ticks(5 * 1000000));

  // Passar para modo texto
  if(vg_exit() != 0) return 1;
  printf("Modo Texto\n");

  return 0;
}

//Falta ligar ao keyboard (ESC 0x81)
int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
  vbe_mode_info_t vmi;

  // Obter informações sobre o modo gráfico
  if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

  // Passar para modo gráfico
  if(vbe_set_mode(mode) != 0) return 1;

  // Mapear a VRAM (frame buffer) para o processo
  if (frame_buffer_init(mode) != 0) return 1;

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

  tickdelay(micros_to_ticks(5 * 1000000));

  // Passar para modo texto
  if(vg_exit() != 0) return 1;
  printf("Modo Texto\n");

  return 0;
}

int (video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  // Passar para modo gráfico
  if(vbe_set_mode(0x105) != 0) return 1;

  // Mapear a VRAM (frame buffer) para o processo
  if (frame_buffer_init(0x105) != 0) return 1;

  // Carrega o XPM
  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  img.bytes = colors;

  //Desenha o XPM
  if(vg_draw_xpm(x,y, &img) != 0) return 1;

  tickdelay(micros_to_ticks(5 * 1000000));

  // Passar para modo texto
  if(vg_exit() != 0) return 1;
  printf("Modo Texto\n");

  return 0;
}

