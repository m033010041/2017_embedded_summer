#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t func(uint32_t x) {
    uint32_t n = x;
    int i,j,k;
    int shift, mask;
    for ( i=0; i<=4; i++ ){
        shift = 1 <<i;
        mask = 1;
        for (j=0; j<32; j=j+(shift*2)){
            for (k=0; k<shift; k++){
                mask = mask<<1;
                mask++;
            }
            mask = mask<<shift;
        }
        n = ((n & mask) >> shift) | ((n & ~mask) << shift);
    }
    return n;
}

uint16_t func(uint16_t x) {
    uint16_t n = x;
    inti,j,k;
    int shift, mask;
    for ( i=0; i<=3; i++ ){
        shift = 1 <<i;
        mask = 1;
        for (j=0; j<16; j=j+(shift*2) ){
            for (k=0; k<shift; k++){
                mask = mask<<1;
                mask++;
            }
            mask = mask<<shift;
        }
        n = ((n & mask) >> shift) | ((n & ~mask) << shift);
    }
    return n;
}

int main()
{
    
    return 0;
}
