#include <stdint.h>

#include "oboe/macros.h"
#include "oboe/drivers/lcd_PL110.h"

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

struct PL110 lcd;

void kernel_main(void) {

    pl110_init();
    pl110_init_dev(&lcd, (void*)0xC0000000);

    pl110_draw_char(&lcd, 0, 0, 'H');
    pl110_draw_char(&lcd, 0, 1, 'e');
    pl110_draw_char(&lcd, 0, 2, 'l');
    pl110_draw_char(&lcd, 0, 3, 'l');
    pl110_draw_char(&lcd, 0, 4, 'o');
    pl110_draw_char(&lcd, 0, 5, ' ');
    pl110_draw_char(&lcd, 0, 6, 'W');
    pl110_draw_char(&lcd, 0, 7, 'o');
    pl110_draw_char(&lcd, 0, 8, 'r');
    pl110_draw_char(&lcd, 0, 9, 'l');
    pl110_draw_char(&lcd, 0, 10, 'd');
    pl110_draw_char(&lcd, 0, 11, '!');

    UART0->CR |= UART_EN; // enable uart0

    uart_write(UART0, buffer);

    while (1) {
        uart_readline(UART0, buffer, BUFFER_SIZE - 1);
        uart_write(UART0, buffer);
    }

}
