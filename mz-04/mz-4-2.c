#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <endian.h>
#include <limits.h>

enum
{
    BUF_SIZE = 2,
    FILE_MODE = 0600,
};

int
main(int argc, char* argv[])
{
    unsigned short num;
    int fd;
    char buf[BUF_SIZE];
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE);

    while (scanf("%hu", &num) == 1) {
        buf[0] = (char)(num >> CHAR_BIT);
        buf[1] = (char)(num);
        write(fd, buf, sizeof(buf));
    }
    return 0;
}