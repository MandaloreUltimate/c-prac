#include <stdio.h>
#include <stdlib.h>

enum
{
    BASE_32 = 32,
    BASE_16 = 16,
    SHIFT_1 = 8,
    SHIFT_2 = 4,
    SHIFT_3 = 16
};

int
main(int argc, char *argv[])
{
    unsigned long long total = 0;
    for (int i = 1; i < argc; i++) {
        unsigned long long cur = (unsigned long long) strtoul(argv[i], NULL, BASE_32);
        total ^= (unsigned long long) cur;
    }
    printf("%x\n", (unsigned int) (((total >> SHIFT_1) % BASE_16) *(1 << SHIFT_2) + (total >> SHIFT_3) % BASE_16));
    return 0;
}