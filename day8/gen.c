#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return 1;
    }
    size_t i, j, nl = 0, size = atoi(argv[1]);
    srand(size);
    char *buf = malloc(sizeof(char) * (size + 1) * size);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            buf[nl + j + size * i] = (rand() % 10) + '0';
            if (j + 1 == size) {
                buf[(++nl) + j + size * i] = '\n';
            }
        }
    }
    fputs(buf, stdout);
    fflush(stdout);
    free(buf);
    return 0;
}
