#include <stdio.h>
#include <string.h>
#include <inttypes.h>
// #include <stdlib.h>

// static inline uint32_t score(char c) {
//     return ((c >= 'a') ? (c - 'a') + 1 : (c - 'A') + 27);
// }

// static inline uint32_t bvec_c(char c) {
//     return (1 << (c - 'A'));
// }

// static inline uint32_t bvec_s(char c) {
//     return (1 << (c - 'a'));
// }

#define score(c) ((c >= 'a') ? (c - 'a') + 1 : (c - 'A') + 27)
#define bvec_c(c) (1 << (c - 'A'))
#define bvec_s(c) (1 << (c - 'a'))

uint32_t sum = 0;
uint32_t g_sum = 0;
char c, buf[64];
int buf_n;
int len, pivot, i;
uint32_t t;
uint32_t left_c, right_c, current_c, group_c = ~0, ruck_c;
uint32_t left_s, right_s, current_s, group_s = ~0, ruck_s;

// char *itoa(uint32_t n) {
//     char *buf = malloc(sizeof(char) * 33);
//     buf[32] = '\0';
//     int i;
//     for (i = 0; i < 32; i++) {
//         buf[32 - i - 1] = ((1 << i) & n) > 0 ? '1' : '0';
//     }
//     return buf;
// }

static void score_group() {
    for (i = 31; i >= 0; i--) {
        if ((char)(i + 64) == 'J') continue;
        t = i > 0 ? 1 << (i - 1) : 1;
        if ((t & group_s) > 0) {
            // printf("'%c': %d\n", (char)(i + 96), i);
            g_sum += i;
            goto end;
        } else if ((t & group_c) > 0) {
            // printf("'%c': %d\n", (char)(i + 64), i + 26);
            g_sum += i + 26;
            goto end;
        }
    }
    i = 10;
    // printf("'%c': %d\n", (char)(i + 64), i + 26);
    g_sum += 10 + 26;
    goto end;
    end: {
        group_c = ~0;
        group_s = ~0;
    }
}

static void score_rucksack() {
    left_c = 0;
    left_s = 0;
    right_c = 0;
    right_s = 0;
    len = strlen(buf) - 1; //don't read newlines.

    ruck_s = 0;
    ruck_c = 0;
    for (i = 0; i < len; i++) {
        if (buf[i] > 96) {
            ruck_s |= bvec_s(buf[i]);
        } else {
            ruck_c |= bvec_c(buf[i]);
        }
    }
    group_s &= ruck_s;
    group_c &= ruck_c;

    pivot = len / 2;
    for (i = 0; i < pivot; i++) {
        if (buf[i] > 96) {
            left_s |= bvec_s(buf[i]);
        } else {
            left_c |= bvec_c(buf[i]);
        }
    }

    for (i = pivot; i < len; i++) {
        if (buf[i] > 96) {
            current_s = bvec_s(buf[i]);
            if ((right_s & current_s) > 0) continue;
            if ((left_s & current_s) > 0) {
                right_s |= current_s;
                sum += score(buf[i]);
            }
        } else {
            current_c = bvec_c(buf[i]);
            if ((right_c & current_c) > 0) continue;
            if ((left_c & current_c) > 0) {
                right_c |= current_c;
                sum += score(buf[i]);
            }
        }
    }
}
int count = 0;

int main(void)
{
    while (fgets(buf, 64, stdin) != NULL)
    {
        score_rucksack();
        count++;
        if (count % 3 == 0) {
            score_group();
        }
    }
    
    printf("sum: %d\n", sum);
    printf("g_sum: %d\n", g_sum);
    return 0;
}
