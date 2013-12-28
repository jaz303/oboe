#ifndef __OBOE_MACROS_H__
#define __OBOE_MACROS_H__

#define __R     volatile const
#define __W     volatile
#define __RW    volatile

//
// Colors

#define BGR_16_565(r, g, b) \
    (((b)&0x1f)<<11)|(((g)&0x1f)<<6)|(((r)&0x1f))

#define BGR_16_565_D(r, g, b) \
    BGR_16_565((r) >> 3, (g) >> 2, (b) >> 3)

#endif