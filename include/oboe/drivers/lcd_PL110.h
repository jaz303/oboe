#ifndef __OBOE_DRIVERS_LCD_PL11O_H__
#define __OBOE_DRIVERS_LCD_PL11O_H__

#include <stdint.h>

#include "oboe/macros.h"

struct PL110_HW {
    __RW    uint32_t        Timing0;            // 0x000 horizontal axis panel control
    __RW    uint32_t        Timing1;            // 0x004 vertical axis panel control
    __RW    uint32_t        Timing2;            // 0x008 clock and signal polarity control
    __RW    uint32_t        Timing3;            // 0x00C line end control
    __RW    uint32_t        UPBASE;             // 0x010 upper panel frame base address
    __RW    uint32_t        LPBASE;             // 0x014 lower panel frame base address
    __RW    uint32_t        INTRENABLE;         // 0x018 interrupt enable mask
    __RW    uint32_t        Control;            // 0x01C LCD panel pixel params
    __RW    uint32_t        Status;             // 0x020 raw interrupt status
    __R     uint32_t        Interrupt;          // 0x024 final masked interrupts
    __R     uint32_t        UPCURR;             // 0x028 LCD upper panel current address value
    __R     uint32_t        LPCURR;             // 0x02C LCD lower panel current address value
            uint32_t        RESERVED0[116];
    __RW    uint32_t        Palette[128];       // 0x200 256x16 bit color palette
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