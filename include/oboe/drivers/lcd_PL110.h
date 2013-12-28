#ifndef __OBOE_DRIVERS_LCD_PL11O_H__
#define __OBOE_DRIVERS_LCD_PL11O_H__

#include <stdint.h>

#include "oboe/macros.h"

struct PL110_HW {
    __RW    uint32_t        Timing0;            // 0x00 horizontal axis panel control
    __RW    uint32_t        Timing1;            // 0x04 vertical axis panel control
    __RW    uint32_t        Timing2;            // 0x08 clock and signal polarity control
    __RW    uint32_t        Timing3;            // 0x0C line end control
    __RW    uint32_t        UPBASE;             // 0x10 upper panel frame base address
    __RW    uint32_t        LPBASE;             // 0x14 lower panel frame base address
    __RW    uint32_t        IMSC;               // 0x18 interrupt enable mask
    __RW    uint32_t        Control;            // 0x1C LCD panel pixel params
    __R     uint32_t        RIS;                // 0x20 raw interrupt status
    __R     uint32_t        MIS;                // 0x24 final masked interrupts
    __W     uint32_t        ICR;                
    __R     uint32_t        UPCURR;             // LCD upper panel current address value
    __R     uint32_t        LPCURR;             // LCD lower panel current address value
            uint32_t        RESERVED0[115];
    __RW    uint32_t        Palette;
};

struct PL110 {
    struct PL110_HW         *hw;
    int                     width;
    int                     height;
};

void pl110_init();
void pl110_init_dev(struct PL110 *lcd, void *base);
void pl110_clear(struct PL110 *lcd, uint8_t r, uint8_t g, uint8_t b);
void pl110_draw_char(struct PL110 *lcd, int y, int x, char c);

#endif