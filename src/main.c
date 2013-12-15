#include <stdint.h>

#define UNUSED(x) (void)(x)

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
}