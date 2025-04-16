#include <lcom/lab5.h>
#include <lcom/lcf.h>

#include "video.h"


char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
unsigned h_res;	        /* Horizontal resolution in pixels */
unsigned v_res;	        /* Vertical resolution in pixels */
unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int (vbe_set_mode)(uint16_t mode){
    reg86_t r;

    memset(&r, 0, sizeof(r));
    r.intno = 0x10;
    // r.ax = 0x4F02; // Podia usar isto ou as duas abaixo (equivalente)
    r.ah = 0x4F;
    r.al = 0x02;

    r.bx = mode | BIT(14);

    if(sys_int86(&r) != OK){
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int (frame_buffer_init)(uint16_t mode) {
    struct minix_mem_range mr;
    unsigned int vram_base;
    unsigned int vram_size;
    int r;				    

    vbe_mode_info_t vmi;
    // Obter informações sobre o modo gráfico
    if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

    // Inicializar a base e o tamanho da VRAM
    vram_base = vmi.PhysBasePtr;
    vram_size = vmi.XResolution * vmi.YResolution * ((vmi.BitsPerPixel + 7) / 8); // Cálculo do tamanho da VRAM

    mr.mr_base = vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    }

    // Mapear a memória da VRAM para o espaço de endereço do processo
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if (video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
    }

    // Inicializar as variáveis globais de resolução e bits por pixel
    h_res = vmi.XResolution;
    v_res = vmi.YResolution;
    bits_per_pixel = vmi.BitsPerPixel;

    return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for (uint16_t i = x; i < x + width; i++) {
        for (uint16_t j = y; j < y + height; j++) {
            uint32_t pixel_offset = (j * h_res + i) * (bits_per_pixel / 8);
            uint8_t* pixel_ptr = (uint8_t*) video_mem + pixel_offset;
    
            // Colorir o pixel
            *(uint32_t*) pixel_ptr = color;
        }
    }
    return 0;
}
