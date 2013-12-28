#include "oboe/drivers/lcd_PL110.h"
#include "oboe/font.h"
#include "oboe/macros.h"

#include <stdint.h>

static uint16_t palette[] = {
    BGR_16_565(0x00, 0x20, 0x10),
    BGR_16_565(0x1F, 0x1F, 0x00)
};

void pl110_init() {

}

// #define PL110_CR_EN     0x001
// #define PL110_CR_PWR        0x800
// #define PL110_PALBASE       (PL110_IOB

void pl110_init_dev(struct PL110 *lcd, void *base) {
    
    struct PL110_HW *hw = lcd->hw = (struct PL110_HW*)base;

    /* 640x480 pixels */
    hw->Timing0 = 0x3f1f3f9c;
    hw->Timing1 = 0x080b61df;
    hw->UPBASE = 0x200000;

    /* 16 bit color */
    hw->Control = 0x1829;

    /* width and height */
    lcd->width = 640;
    lcd->height = 480;

    /* clear */
    pl110_clear(lcd, 0, 128, 128);

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