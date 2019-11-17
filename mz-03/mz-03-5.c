#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

char
*getline2(FILE *f);

enum
{
    DEC_BASE = 10,
    SPACES_INPUT = 0x0bad1dea,
    NO_NUM_INPUT = 0xbedabeda
};

int
main(void)
{
    char *str;
    unsigned str_cnt = 1;
    while ((str = getline2(stdin))) {

        unsigned num_cnt = 0;
        unsigned sum = 0;
        int flag = 0;
        char *pos = str;

        while (*pos != '\n' && *pos) {

            if (isdigit((unsigned char) *pos)
                    || ((*pos == '-' || *pos == '+') && isdigit((unsigned char) pos[1]))) {

                num_cnt++;
                long num = strtol(pos, &pos, DEC_BASE);

                if (errno == ERANGE || num > INT_MAX || num < INT_MIN || num != (int) num) {
                    if (num < 0) {
                        num = -num_cnt;
                    } else if (num > 0) {
                        num = num_cnt;
                    }
                }

                sum += (unsigned) num;
                errno = 0;

            } else if (isspace((unsigned char) *pos)) {
                pos++;
            } else {
                flag = 1;
                break;
            }
        }

        int res = (int) sum;
        if (flag) {
            res = str_cnt + NO_NUM_INPUT;
        } else if (!num_cnt) {
            res = str_cnt + SPACES_INPUT;
        }

        printf("%d\n", res);
        free(str);
        str_cnt++;
    }
    return 0;
}
