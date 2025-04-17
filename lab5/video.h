#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>

int (vbe_set_mode)(uint16_t mode);
int (frame_buffer_init)(uint16_t mode);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (color_pixel)(uint8_t* pixel_ptr, uint32_t color);
uint32_t (calculate_indexed_color)(uint8_t row, uint8_t col, uint32_t first, uint8_t step, uint8_t bits_per_pixel, uint8_t no_rectangles);
uint32_t (calculate_direct_color)(uint8_t row, uint8_t col, uint32_t first, uint8_t step, vbe_mode_info_t vmi);
int (vg_draw_xpm)(uint16_t x, uint16_t y, xpm_image_t *img);

#endif /* VIDEO_H */

