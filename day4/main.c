// #include <stdio.h>
// #include <inttypes.h>
// #include <stdlib.h>
// #include <string.h>

// char *_itoa(uint32_t n) {
//     char *buf = malloc(sizeof(char) * 32);
//     int i;
//     for (i = 0; i < 32; i++) {
//         buf[32 - i - 1] = ((1 << i) & n) > 0 ? '1' : '0';
//     }
//     return buf;
// }

// char *itoa4(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
//     char *buf = malloc(sizeof(char) * 129);
//     buf[128] = '\0';
//     memcpy(buf, _itoa(d), 32);
//     memcpy(buf + 32, _itoa(c), 32);
//     memcpy(buf + 64, _itoa(b), 32);
//     memcpy(buf + 96, _itoa(a), 32);
//     return buf;
// }

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
    #define set_and_of_two4(v, a, b) do { v##1 = a##1 & b##1; v##2 = a##2 & b##2; v##3 = a##3 & b##3; v##4 = a##4 & b##4; } while(0)
    #define sub_eq_zero4(a, b) ((a##1 - b##1) == 0 && (a##2 - b##2) == 0 && (a##3 - b##3) == 0 && (a##4 - b##4) == 0)
    #define itoa(v) itoa4(v##1, v##2, v##3, v##4)
    uint32_t count = 0;
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
        // printf("%02d-%02d: %s\n", l1, r1, itoa(fst));
        // printf("%02d-%02d: %s\n", l2, r2, itoa(snd));
        set_and_of_two4(intrs, fst, snd);
        if (sub_eq_zero4(fst, intrs) || sub_eq_zero4(snd, intrs)) {
            count++;
            // printf("intrs: %s\n\n", itoa(intrs));
        }
        // } else {
            // printf("\n");
        // }
    }
    printf("count: %d\n", count);
}