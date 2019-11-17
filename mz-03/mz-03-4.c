#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

enum
{
    BUF_SIZE = 16,
    DEC_BASE = 10
};

int
main(void)
{
    char buf[BUF_SIZE];
    unsigned long long curr_num = 0;
    unsigned long long sum = 0;
    ssize_t read_bytes;
    char sign = '+';

    while ((read_bytes = read(STDIN_FILENO, buf, sizeof(buf)))) {
        if(read_bytes == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }

        for (int i = 0; i < read_bytes; i++) {
            if (buf[i] >= '0' && buf[i] <= '9') {
                curr_num = curr_num * DEC_BASE + buf[i] - '0';
            } else if (buf[i] == '+' || buf[i] == '-') {
                sign = buf[i];
            } else if (isspace((unsigned char) buf[i])) {
                if (sign == '+') {
                    sum += curr_num;
                } else if (sign == '-') {
                    sum -= curr_num;
                }
                sign = '+';
                curr_num = 0;
            }
        }
    }

    if (sign == '+') {
        sum += curr_num;
    } else if (sign == '-') {
        sum -= curr_num;
    }

    printf("%lld\n", (long long) sum);
    return 0;
}