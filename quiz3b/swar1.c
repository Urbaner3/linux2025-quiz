#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool both_odd(uint32_t x, uint32_t y)
{
    return (x & 1) && (y & 1);
}

static uint64_t SWAR_ODD_MASK = (1L << 32) + 1;
bool both_odd_swar(uint64_t xy) {
    return (xy & SWAR_ODD_MASK) == SWAR_ODD_MASK;
}

static inline uint64_t bit_compound(uint32_t x, uint32_t y)
{
    return ((0L + x) << 32) | ((y + 0L) & (-1L >> 32));
}

int main()
{
    int x = 12345678;
    int y = 9012345;
    uint64_t xy = bit_compound(x, y);
    printf("%d, %d\n", both_odd(x, y), both_odd_swar(xy));
}
