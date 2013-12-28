#include "oboe/drivers/lcd_PL110.h"
#include "oboe/font.h"
#include "oboe/macros.h"

#include <stdint.h>

static uint16_t palette[] = {
    BGR_16_565(0x1F, 0x00, 0x00),
    BGR_16_565(0x1F, 0x1F, 0x00)
};

// static void pl110_init(struct PL110 *lcd, void *addr) {
//     lcd->hw = (struct PL110_HW*)addr;
// }

// static void pl110_clear(struct PL110 *lcd) {

// }

void pl110_init() {

}

// #define PL110_CR_EN     0x001
// #define PL110_CR_PWR        0x800
// #define PL110_IOBASE        0xc0000000
// #define PL110_PALBASE       (PL110_IOB

// typedef struct _PL110MMIO 
// {
//     uint32      volatile tim0;      //0
//     uint32      volatile tim1;      //4
//     uint32      volatile tim2;      //8
//     uint32      volatile d;     //c
//     uint32      volatile upbase;    //10
//     uint32      volatile f;     //14
//     uint32      volatile g;     //18
//     uint32      volatile control;   //1c
// } PL110MMIO;

void pl110_init_dev(struct PL110 *lcd, void *base) {
    lcd->hw = (struct PL110_HW*)base;

    // PFN     fn;
    // PL110MMIO   *plio;
    // int     x;
    // uint16      volatile *fb;
    
    // plio = (PL110MMIO*)PL110_IOBASE;
    
    // /* 640x480 pixels */
    // plio->tim0 = 0x3f1f3f9c;
    // plio->tim1 = 0x080b61df;
    // plio->upbase = 0x200000;
    // /* 16-bit color */
    // plio->control = 0x1829;
    // fb = (uint16*)0x200000;
    // for (x = 0; x < (640 * 480) - 10; ++x) {
    //     fb[x] = RGB(0x1f,0,0);
    // }
}

void pl110_clear(struct PL110 *lcd, uint8_t r, uint8_t g, uint8_t b) {
    
    uint16_t volatile *fb   = (uint16_t*)lcd->hw->UPBASE;
    uint16_t color          = BGR_16_565_D(r, g, b);
    uint32_t max            = lcd->width * lcd->height;

    for (uint32_t i = 0; i < max; ++i) {
        fb[i] = color;
    }

}

void pl110_draw_char(struct PL110 *lcd, int y, int x, char c) {

    // get the base address of the font
    char *font = (char*)(&_font_start);

    // now calculate the base address of the character
    // each char is 8x16 bits == 16 bytes
    font += (c * 16);

    uint16_t volatile *fb = (uint16_t*)lcd->hw->UPBASE;

    // now calculate offset of top-left pixel
    fb += (16*640*y)+(8*x);

    // finally copy character onto screen buffer
    // TODO: this is revoltingly slow...!
    for (int j = 0; j < 16; ++j) {
        uint8_t line = font[j];
        fb[j*640 + 0] = palette[(line >> 7) & 1];
        fb[j*640 + 1] = palette[(line >> 6) & 1];
        fb[j*640 + 2] = palette[(line >> 5) & 1];
        fb[j*640 + 3] = palette[(line >> 4) & 1];
        fb[j*640 + 4] = palette[(line >> 3) & 1];
        fb[j*640 + 5] = palette[(line >> 2) & 1];
        fb[j*640 + 6] = palette[(line >> 1) & 1];
        fb[j*640 + 7] = palette[(line >> 0) & 1];
    }

}