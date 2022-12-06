#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _stack { uint8_t cargo[64]; uint8_t height; } stack;
#define push(s, b) s.cargo[s.height++] = b
#define pop(s) s.cargo[--s.height]

int main(int argc, char **argv) {
    char buf[64], *stack_lines[64];
    uint8_t c, amount, from, to, part = 1;
    stack *stacks;
    uint32_t stack_lines_n = 0, stacks_n = 0;
    int16_t i, j;
    if (argc == 2) {
        part = atoi(argv[1]);
        if (part < 1 || part > 2) {
            printf("Invalid input, there's no part %d, what are you on about???\n", part); return 1;
        }
    }
    while (1) {
        fgets(buf, 64, stdin);
        if (stacks_n == 0) {
            stacks_n = strlen(buf) / 4;
        }
        if (buf[1] == '1') {
            break;
        }
        stack_lines[stack_lines_n] = calloc(64, sizeof(char));
        strncpy(stack_lines[stack_lines_n], buf, 64);
        stack_lines_n++;
    }
    stacks = calloc(stacks_n, sizeof(stack));
    for (i = stack_lines_n - 1; i >= 0; i--) {
        for (j = 0; j < stacks_n; j++) {
            c = stack_lines[i][(j * 4) + 1];
            if (c > 64) {
                push(stacks[j], c);
            }
        }
        free(stack_lines[i]);
    }
    fgets(buf, 64, stdin); // Drop one line.
    while (fgets(buf, 64, stdin) != NULL) {
        if (sscanf(buf, "move %hhd from %hhd to %hhd\n", &amount, &from, &to) < 0) break;
        to--; from--;
        if (part == 1) {
            for (c = 0; c < amount; c++) {
                push(stacks[to], pop(stacks[from]));
            }
        } else {
            uint8_t carrier[64];
            for (c = 0; c < amount; c++) {
                carrier[c] = pop(stacks[from]);
            }
            for (c = 0; c < amount; c++) {
                push(stacks[to], (carrier[amount - 1 - c]));
            }
        }
    }
    for (i = 0; i < stacks_n; i++) {
        printf("%c", stacks[i].cargo[stacks[i].height - 1]);
    }
    printf("\n");
    free(stacks);
    return 0;
}
