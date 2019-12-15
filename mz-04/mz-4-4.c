#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Not enough parameters passed\n");
        return 1;
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR)) < 0) {
        perror("Failed to open the file: ");
        return 1;
    }

    signed long long num, min;
    ssize_t read_bytes;
    if ((read_bytes = read(fd, &min, sizeof(num))) < 0) {
        perror("Failed to read the data: ");
        close(fd);
        return 1;
    } else if (read_bytes != sizeof(min)) {
        close(fd);
        return 0;
    }

    off_t cur_offset = sizeof(min);
    off_t min_offset = 0;

    while (read(fd, &num, sizeof(num)) == sizeof(num)) {
        if (num < min) {
            min = num;
            min_offset = cur_offset;
        }
        cur_offset += sizeof(num);
    }

    min = -(unsigned long long) min;
    if(lseek(fd, min_offset, SEEK_SET) < 0) {
        perror("Failed to find the minimum number offset: ");
        close(fd);
        return 0;
    }
    if (write(fd, &min, sizeof(min)) != sizeof(min)) {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}