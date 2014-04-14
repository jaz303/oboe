#ifndef __OBOE_UTIL_MEMCPY__
#define __OBOE_UTIL_MEMCPY__

#include <stdint.h>

void* kmemset(void *ptr, int value, uint32_t num);
void* kmemset_v(void volatile *ptr, int value, uint32_t num);

/*
 * beginning at address dst, fill memory with num copies of word val
 */
void kmemfill_w(uint32_t *dst, uint32_t val, int32_t num);

/*
 * beginning at volatile address dst, fill memory with num copies of word val
 */
void kmemfill_wv(uint32_t volatile *dst, uint32_t val, int32_t num);

#endif