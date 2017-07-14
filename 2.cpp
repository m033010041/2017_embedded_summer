#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


uint32_t half_add(uint32_t a, uint32_t b) {
    if (b == 0) return a;
    uint32_t sum = a ^ b;             /* 相加但不進位 */
    uint32_t carry = (a & b) << 1;    /* 進位但不相加 */
    return half_add(sum, carry);
}

uint32_t mul(uint32_t a, uint32_t b) {
    if (b == 1) return a;
    if (b == 0) return 0;
    if (b & 1)
        return half_add(a ,(mul( a, b>>1 )<<1));
    else
        return half_add(0 ,(mul( a, b>>1 )<<1));
}
