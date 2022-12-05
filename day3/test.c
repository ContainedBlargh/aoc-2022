#include <stdio.h>
#include <inttypes.h>
char _itoab[65];

static void itoa(uint64_t n)
{
    if (n > 1)
        itoa(n >> 1);
    printf("%ld", n & 1);
}
int main(void) {
    itoa(1 << 5);
}