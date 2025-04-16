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

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  vbe_mode_info_t vmi;

  // Obter informações sobre o modo gráfico
  if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

  // Passar para modo gráfico
  if(vbe_set_mode(mode) != 0) return 1;

  // Mapear a VRAM (frame buffer) para o processo
  if (frame_buffer_init(mode) != 0) return 1;

  // Desenhar o retângulo no frame buffer
  draw_rectangle(x,y,width,height,color);


  tickdelay(micros_to_ticks(5 * 1000000));

  // Passar para modo texto
  if(vg_exit() != 0) return 1;
  printf("Modo Texto\n");

  return 0;
}
