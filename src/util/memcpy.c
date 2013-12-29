#include "oboe/util/memcpy.h"

// TODO: compare generated assembly between volatile/non-volatile variants
// TODO: loop unrolling
// TODO: rewrite in assembly?

void kmemfill_w(uint32_t *dst, uint32_t val, int32_t num) {
    for (int32_t i = 0; i < num; ++i) {
        dst[i] = val;
    }
}

void kmemfill_wv(uint32_t volatile *dst, uint32_t val, int32_t num) {
    for (int32_t i = 0; i < num; ++i) {
        dst[i] = val;
    }
}
