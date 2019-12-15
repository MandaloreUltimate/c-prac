#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    BUF_SIZE = 1002,
    YEAR_OFFSET = 1900,
};

int
convert(time_t *cur_time, FILE *fd)
{
    char buf[BUF_SIZE];
    if (!fgets(buf, sizeof(buf), fd)) {
        return 0;
    }
    struct tm timestamp;
    strptime(buf, "%Y/%m/%d %H:%M:%S", &timestamp);
    timestamp.tm_isdst = -1;
    *cur_time = mktime(&timestamp);
    return 1;
}

int
main(int argc, char *argv[])
{
    FILE *fd = fopen(argv[1], "r");
    if (fd < 0) {
        return 1;
    }
    time_t prev;
    convert(&prev, fd);
    time_t next;

    while (convert(&next, fd)) {
        printf("%ld\n", next - prev);
        prev = next;
    }
    return 0;
}
