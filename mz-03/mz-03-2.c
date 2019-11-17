#include <stdio.h>
#include <stdlib.h>

enum { DEC_BASE = 10 };

int
main(int argc, char *argv[])
{
    long long pos_sum = 0;
    long long neg_sum = 0;
    long long num;
    char *end;
    for (int i = 1; i < argc; i++) {
        num = strtoll(argv[i], &end, DEC_BASE);
        if(num > 0) {
            pos_sum += num;
        } else if(num < 0) {
            neg_sum += num;
        }
    }
    printf("%lld\n%lld\n", pos_sum, neg_sum);
    return 0;
}