#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <byteswap.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

enum
{
    BUF_SIZE = 2,
};

int
main(int argc, char *argv[])
{
    unsigned short min, num;
    int flag = 0;
    int fd = open(argv[1], O_RDONLY);
    int buf_size = sizeof(unsigned short);
    unsigned char buf[buf_size];
    while (read(fd, &buf, sizeof(buf)) == sizeof(buf)) {
        num = (buf[0] << CHAR_BIT);
        num += buf[1];

        if (!flag) {
            if (num % 2 == 0) {
                flag = 1;
                min = num;
            }
        } else {
            if (num % 2 == 0 && num < min) {
                min = num;
            }
        }
    }
    if (flag) {
        printf("%hu\n", min);
    }
    close(fd);
    return 0;
}
