#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

enum { DEC_BASE = 10 };

int
main(int argc, char *argv[])
{
    unsigned long long size = 0;
    DIR *d = opendir(argv[1]);
    if(!d) {
        return -1;
    }

    unsigned int max = (unsigned int)(strtoul(argv[2], NULL, DEC_BASE));
    struct dirent *dd;
    while ((dd = readdir(d))) {
        struct stat sb;
        char path[PATH_MAX];
        int slen = snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name);
        if (slen + 1 > sizeof(path)) {
            continue;
        }

        if(lstat(path, &sb) < 0 || !S_ISREG(sb.st_mode) || access(path, W_OK) < 0) {
            continue;
        }

        unsigned int num;
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            continue;
        }

        if (read(fd, &num, sizeof(num)) < sizeof(num)) {
            continue;
        }

        close(fd);
        if (num == max) {
            size += sb.st_size;
        }
    }
    printf("%lld\n", size);
    return 0;
}