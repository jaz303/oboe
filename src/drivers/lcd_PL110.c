#include "oboe/drivers/lcd_PL110.h"
#include "oboe/font.h"
#include "oboe/macros.h"
#include "oboe/util.h"

#include <stdint.h>

//
// Control flags

#define PL110_WATERMARK                     (1 << 16)

#define PL110_LDmaFIFOTME                   (1 << 15)
#define PL110_LcdVComp_START_VSYNC          (0 << 12)
#define PL110_LcdVComp_START_BACK_PORCH     (1 << 12)
#define PL110_LcdVComp_START_ACTIVE_VIDEO   (2 << 12)
#define PL110_LcdVComp_START_FRONT_PORCH    (3 << 12)

#define PL110_LcdPwr                        (1 << 11)
#define PL110_BEPO                          (1 << 10)
#define PL110_BEBO                          (1 << 9)
#define PL110_BGR                           (1 << 8)

#define PL110_LcdDual                       (1 << 7)
#define PL110_LcdMono8                      (1 << 6)
#define PL110_LcdTFT                        (1 << 5)
#define PL110_BW                            (1 << 4)

#define PL110_BPP_1                         (0 << 1)
#define PL110_BPP_2                         (1 << 1)
#define PL110_BPP_4                         (2 << 1)
#define PL110_BPP_8                         (3 << 1)
#define PL110_BPP_16                        (4 << 1)
#define PL110_BPP_24                        (5 << 1)
#define PL110_LcdEn                         (1 << 0)

//
// Status flags

// AMBA AHB master bus error status, set when the AMBA AHB master
// encounters a bus error response from a slave. */
#define PL110_MBERROR                       (1 << 4)

// Vertical compare, set when one of the four vertical regions,
// selected via the LCDControl register, is reached.
#define PL110_VComp                         (1 << 3)

// LCD next address base update, mode dependent, set when the current
// base address registers have been successfully updated by the next
// address registers. Signifies that a new next address can be loaded
// if double buffering is in use.
#define PL110_LNBU                          (1 << 2)

// FIFO underflow, set when either the upper or lower DMA FIFOs
// have been read accessed when empty causing an underflow condition
// to occur.
#define PL110_FUF                           (1 << 1)

static uint16_t palette[] = {
    BGR_16_565(0x00, 0x20, 0x10),
    BGR_16_565(0x1F, 0x1F, 0x00)
};

void pl110_init() {

}

void pl110_init_dev(struct PL110 *lcd, void *base) {
    
    struct PL110_HW *hw = lcd->hw = (struct PL110_HW*)base;

    const int width = 1024;
    const int height = 768;

    /* timing registers. i have no idea what i'm doing. */
    hw->Timing0 = 0x3f1f3f00 | (((width / 16) - 1) << 2);
    hw->Timing1 = 0x080b6000 | (height - 1);

    /* framebuffer location - TODO: set aside some space properly in the link script */
    hw->UPBASE = 0x200000;

    /* initialise LCD with 16 BPP, TFT */
    hw->Control = PL110_LcdVComp_START_BACK_PORCH
                    | PL110_LcdPwr
                    | PL110_LcdTFT
                    | PL110_BPP_16
                    | PL110_LcdEn;

    /* width and height */
    lcd->width = width;
    lcd->height = height;

    /* clear */
    pl110_clear(lcd, 0, 128, 128);

}

void pl110_clear(struct PL110 *lcd, uint8_t r, uint8_t g, uint8_t b) {
    
    uint16_t color = BGR_16_565_D(r, g, b);
    
    kmemfill_wv((uint32_t*)lcd->hw->UPBASE,
                (color << 16 | color),
                (lcd->width * lcd->height) / 2);

    // for (uint32_t i = 0; i < max; ++i) {
    //     fb[i] = color;
    // }

}

void pl110_draw_char(struct PL110 *lcd, int y, int x, char c) {

    // lcd width
    const int width = lcd->width;

    // base address of the 8x16 ascii font
    char *font = (char*)(&_font_start);

    // now calculate the base address of the character
    // each char is 8x16 bits == 16 bytes
    font += (c * 16);

    // now calculate offset of top-left pixel
    uint16_t volatile *fb = (uint16_t*)lcd->hw->UPBASE;
    fb += (16*width*y)+(8*x);

    // finally copy character onto screen buffer
    // TODO: this is revoltingly slow...!
    for (int j = 0; j < 16; ++j) {
        uint8_t line = font[j];
        fb[j*width + 0] = palette[(line >> 7) & 1];
        fb[j*width + 1] = palette[(line >> 6) & 1];
        fb[j*width + 2] = palette[(line >> 5) & 1];
        fb[j*width + 3] = palette[(line >> 4) & 1];
        fb[j*width + 4] = palette[(line >> 3) & 1];
        fb[j*width + 5] = palette[(line >> 2) & 1];
        fb[j*width + 6] = palette[(line >> 1) & 1];
        fb[j*width + 7] = palette[(line >> 0) & 1];
    }

}