#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    struct stat sb;
    long long sum_size = 0;
    for (int i = 1; i < argc; i++) {
        if (!lstat(argv[i], &sb) && S_ISREG(sb.st_mode) && sb.st_nlink == 1) {
            sum_size += sb.st_size;
        }
    }
    printf("%lld\n", sum_size);
    return 0;
}