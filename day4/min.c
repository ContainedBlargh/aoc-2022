#include <stdio.h>
#include <inttypes.h>

int main(void) {
    uint8_t l1, r1, l2, r2, i;
    uint32_t fst1, fst2, fst3, fst4;
    uint32_t snd1, snd2, snd3, snd4;
    #define set4(v, expr) do { v##1 expr; v##2 expr; v##3 expr; v##4 expr; } while(0)
    #define set_or4(v, i) do { if (i < 32) { v##1 |= 1 << i; } else if (i < 64) { v##2 |= 1 << (i - 32); } \
       else if (i < 96) { v##3 |= 1 << (i - 64); } else if (i < 128) { v##4 |= 1 << (i - 96); } } while(0)
    uint32_t intrs1, intrs2, intrs3, intrs4;
    #define set_and_of_two4(v, a, b) do { \
         v##1 = a##1 & b##1; v##2 = a##2 & b##2; v##3 = a##3 & b##3; v##4 = a##4 & b##4; \
    } while(0)
    #define sub_eq_zero4(a, b) ((a##1 - b##1) == 0 && (a##2 - b##2) == 0 &&  (a##3 - b##3) == 0 && (a##4 - b##4) == 0)
    #define and_not_zero4(a, b) ((a##1 & b##1) != 0 || (a##2 & b##2) != 0 || (a##3 & b##3) != 0 || (a##4 & b##4) != 0)
    #define itoa(v) itoa4(v##1, v##2, v##3, v##4)
    uint32_t part1 = 0;
    uint32_t part2 = 0;
    while (scanf("%hhd-%hhd,%hhd-%hhd\n", &l1, &r1, &l2, &r2) != EOF)
    {
        set4(fst, = 0);
        set4(snd, = 0);
        for (i = l1; i <= r1; i++) {
            set_or4(fst, i);

        }
        for (i = l2; i <= r2; i++) {
            set_or4(snd, i);
        }
        if (and_not_zero4(fst, snd)) {
            part2++;
        }
        set_and_of_two4(intrs, fst, snd);
        if (sub_eq_zero4(fst, intrs) || sub_eq_zero4(snd, intrs)) {
            part1++;
        }
    }
    printf("part1: %d\n", part1);
    printf("part2: %d\n", part2);
}