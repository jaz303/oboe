#include <stdint.h>

#define __R     volatile const
#define __W     volatile
#define __RW    volatile

// base address of 8x16 font
extern const int _font_start;

//
//

typedef struct PeripheralID {
    __R     uint32_t        PeriphID0;
    __R     uint32_t        PeriphID1;
    __R     uint32_t        PeriphID2;
    __R     uint32_t        PeriphID3;
    __R     uint32_t        PCellID0;
    __R     uint32_t        PCellID1;
    __R     uint32_t        PCellID2;
    __R     uint32_t        PCellID3;
} PeripheralID_t;

//
// LCD

typedef struct {
    __RW    uint32_t        Timing0;            // horizontal axis panel control
    __RW    uint32_t        Timing1;            // vertical axis panel control
    __RW    uint32_t        Timing2;            // clock and signal polarity control
    __RW    uint32_t        Timing3;            // line end control
    __RW    uint32_t        UPBASE;             // upper panel frame base address
    __RW    uint32_t        LPBASE;             // lower panel frame base address
    __RW    uint32_t        IMSC;               // interrupt enable mask
    __RW    uint32_t        Control;            // LCD panel pixel params
    __R     uint32_t        RIS;                // raw interrupt status
    __R     uint32_t        MIS;                // final masked interrupts
    __W     uint32_t        ICR;                
    __R     uint32_t        UPCURR;             // LCD upper panel current address value
    __R     uint32_t        LPCURR;             // LCD lower panel current address value
            uint32_t        RESERVED0[115];
    __RW    uint32_t        Palette;

    __R     PeripheralID_t PeripheralID;

} IntegratorLCD_t;

#define LCD_BASE            0xC0000000
#define LCD                 ((IntegratorLCD_t*) LCD_BASE)

//
// UART

typedef struct {
    __RW    uint32_t        DR;
    union {
    __R     uint32_t        RSR;
    __W     uint32_t        ECR;
    };
            uint32_t        RESERVED0[4];
    __R     uint32_t        FR;
            uint32_t        RESERVED1[1];
    __RW    uint32_t        ILPR;
    __RW    uint32_t        IBRD;
    __RW    uint32_t        FBRD;
    __RW    uint32_t        LCR_H;
    __RW    uint32_t        CR;
    __RW    uint32_t        IFLS;
    __RW    uint32_t        IMSC;
    __R     uint32_t        RIS;
    __R     uint32_t        MIS;
    __W     uint32_t        ICR;
    __RW    uint32_t        DMACR;
            uint32_t        RESERVED2[997];

    __R     PeripheralID_t PeripheralID;

} UART_TypeDef;

#define UART0_BASE          0x16000000
#define UART1_BASE          0x17000000

#define UART0               ((UART_TypeDef*)    UART0_BASE)
#define UART1               ((UART_TypeDef*)    UART1_BASE)

#define UART_RI             (1 << 8)
#define UART_TXFE           (1 << 7)
#define UART_RXFF           (1 << 6)
#define UART_TXFF           (1 << 5)
#define UART_RXFE           (1 << 4)
#define UART_BUSY           (1 << 3)
#define UART_DCD            (1 << 2)
#define UART_DSR            (1 << 1)
#define UART_CTS            (1 << 0)

#define UART_CTSEn          (1 << 15)
#define UART_RTSEn          (1 << 14)
#define UART_Out2           (1 << 13)
#define UART_Out1           (1 << 12)
#define UART_RTS            (1 << 11)
#define UART_DTR            (1 << 10)
#define UART_RXE            (1 << 9)
#define UART_TXE            (1 << 8)
#define UART_LBE            (1 << 7)
#define UART_SIRLP          (1 << 2)
#define UART_SIREN          (1 << 1)
#define UART_EN             (1 << 0)

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE] = {'H','e','l','l','o',',',' ','W','o','r','l','d','!','\n'};

char uart_getc(UART_TypeDef *dev) {
    while (dev->FR & UART_RXFE);
    return dev->DR;
}

void uart_putc(UART_TypeDef *dev, char c) {
    while (dev->FR & UART_TXFF);
    dev->DR = c;   
}

int uart_readline(UART_TypeDef *dev, char *buffer, int maxlen) {

    int nread = 0;

    while (nread < maxlen) {
        char ch = uart_getc(dev);
        if (ch == '\r' || ch == '\n') {
            buffer[nread++] = '\n';
            break;
        } else {
            buffer[nread++] = ch;
        }
    }

    buffer[nread] = 0;

    return nread;

}

void uart_write(UART_TypeDef *dev, const char *str) {
    while (*str) {
        uart_putc(dev, (*str++));
    }
}

typedef int(*PFN)(void);
 
void start(void);

 
#define PL110_CR_EN     0x001
#define PL110_CR_PWR        0x800
#define PL110_IOBASE        0xc0000000
#define PL110_PALBASE       (PL110_IOBASE + 0x200)
 
typedef unsigned int        uint32;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
 
typedef struct _PL110MMIO 
{
    uint32      volatile tim0;      //0
    uint32      volatile tim1;      //4
    uint32      volatile tim2;      //8
    uint32      volatile d;     //c
    uint32      volatile upbase;    //10
    uint32      volatile f;     //14
    uint32      volatile g;     //18
    uint32      volatile control;   //1c
} PL110MMIO;

#define RGB(r,g,b) \
    (((b)&0x1f)<<11)|(((g)&0x1f)<<6)|(((r)&0x1f))

uint16_t palette[] = {
    RGB(0x1f, 0x00, 0x00),
    RGB(0x1f, 0x1f, 0x00)
};
 
void console_draw_char(int y, int x, char c) {

    // get the base address of the font
    char *font = (char*)(&_font_start);

    // now calculate the base address of the character
    // each char is 8x16 bits == 16 bytes
    font += (c * 16);

    // framebuffer base
    uint16 volatile *fb = (uint16_t*)0x200000;

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

void start(void)
{
    PFN     fn;
    PL110MMIO   *plio;
    int     x;
    uint16      volatile *fb;
 
    plio = (PL110MMIO*)PL110_IOBASE;
 
    /* 640x480 pixels */
    plio->tim0 = 0x3f1f3f9c;
    plio->tim1 = 0x080b61df;
    plio->upbase = 0x200000;
    /* 16-bit color */
    plio->control = 0x1829;
    fb = (uint16*)0x200000;
    for (x = 0; x < (640 * 480) - 10; ++x) {
        fb[x] = RGB(0x1f,0,0);
    }

    console_draw_char(0, 0, 'H');
    console_draw_char(0, 1, 'e');
    console_draw_char(0, 2, 'l');
    console_draw_char(0, 3, 'l');
    console_draw_char(0, 4, 'o');
    console_draw_char(0, 5, ' ');
    console_draw_char(0, 6, 'W');
    console_draw_char(0, 7, 'o');
    console_draw_char(0, 8, 'r');
    console_draw_char(0, 9, 'l');
    console_draw_char(0, 10, 'd');
    console_draw_char(0, 11, '!');

    /* uncomment this and the function pointer should crash QEMU if you set it for 8MB of ram or less */
    // for(;;);
    // fn = (PFN)0x800f20;
    // fn();
    return;
}

void kernel_main(void) {

    UART0->CR |= UART_EN; // enable uart0

    uart_write(UART0, buffer);

    start();

    while (1) {
        uart_readline(UART0, buffer, BUFFER_SIZE - 1);
        uart_write(UART0, buffer);
    }

}
