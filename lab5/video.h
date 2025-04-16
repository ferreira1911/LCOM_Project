#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>

int (vbe_set_mode)(uint16_t mode);
int (frame_buffer_init)(uint16_t mode);
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif /* VIDEO_H */

