#ifndef __OBOE_DEVICES_FRAMEBUFFER_H__
#define __OBOE_DEVICES_FRAMEBUFFER_H__

typedef struct {

} kframebuffer_t;

void kframebuffer_init(kframebuffer_t *fb);
void kframebuffer_clear(kframebuffer_t *fb);
void kframebuffer_draw_ascii_char(kframebuffer_t *fb, int row, int col, char ch);

#endif