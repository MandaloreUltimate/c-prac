#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

enum
{
    FILE_MODE = 0660,
    DEC_BASE = 10,
    HEX_BASE = 16
};

unsigned long long
hash_function(unsigned long long);

int
main(int argc, char *argv[])
{
    char file_path[PATH_MAX];
    sscanf(argv[1], "%s", file_path);
    unsigned long long h_0 = strtoull(argv[2], NULL, HEX_BASE);
    unsigned long long cnt = strtoull(argv[3], NULL, DEC_BASE);
    unsigned long long h = h_0;

    int fd;
    if (!(fd = open(file_path, O_WRONLY | O_CREAT, FILE_MODE))) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    off_t pos;
    if ((pos = lseek(fd, 0, SEEK_END)) < 0) {
        close(fd);
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    for (unsigned long long i = 0; i < cnt; i++) {
        if (lseek(fd, pos + (cnt - (i + 1)) * sizeof(h), SEEK_SET) < 0) {
            close(fd);
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }

        if (write(fd, &h, sizeof(h)) != sizeof(h)) {
            close(fd);
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }

        h = hash_function(h);
    }

    close(fd);
    return 0;
}