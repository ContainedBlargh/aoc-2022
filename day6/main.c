#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// http://graphics.stanford.edu/~seander/bithacks.html
static uint32_t bitcount(uint32_t v) {
    uint32_t c;
    v = v - ((v >> 1) & 0x55555555);
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    c = (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
    return c;
}

#define ring_push(arr, sz, elem) do { arr[arr##_i] = elem; arr##_i = (arr##_i + 1) % sz; } while(0)
#define ring_or4(arr) arr[0] | arr[1] | arr[2] | arr[3]
#define ring_or4O(arr, o) arr[0 + o] | arr[1 + o] | arr[2 + o] | arr[3 + o]
#define ring_or14(arr) ring_or4O(arr, 0) | ring_or4O(arr, 4) | ring_or4O(arr, 8) | arr[12] | arr[13]

int main(void) {
    uint32_t i = 0, pbc = 0, mbc = 0, first_p = 0, first_m = 0;
    uint32_t pbuf[4] = {0}, mbuf[14] = {0}, alpha;
    uint8_t pbuf_i = 0, mbuf_i = 0, c;
    while (1)
    {
        i++;
        c = fgetc(stdin);
        if (c < 96) {
            break;
        }
        alpha = 1 << (c - 'a');
        ring_push(pbuf, 4, alpha);
        ring_push(mbuf, 14, alpha);
        pbc = bitcount(ring_or4(pbuf));
        mbc = bitcount(ring_or14(mbuf));
        if (pbc == 4 && first_p == 0) {
            first_p = i;
        }
        if (mbc == 14 && first_m == 0) {
            first_m = i;
        }
        if (first_p != 0 && first_m != 0) {
            break;
        }
    }
    printf("part1: %d\n", first_p);
    printf("part2: %d\n", first_m);
    return 0;
}