#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

enum
{
    STR_LEN = 16,
    COST_LEN = 4
};

typedef struct Cheque
{
    char name[STR_LEN];
    int cost;
} Cheque;

int main(int argc, char *argv[])
{
    int max = 0;
    int max_2 = 0;
    int max_flag = 0;
    int max_2_flag = 0;
    int num, fd;
    Cheque tmp;
    for (int i = 1; i < argc; i++) {
        if (!(fd = open(argv[i], O_RDONLY))) {
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }

        for (;;) {
            if (read(fd, &tmp, sizeof(tmp)) < sizeof(tmp)) {
                break;
            }
            num = tmp.cost;
            if (max_flag && max_2_flag) {
                if (num > max) {
                    max_2 = max;
                    max = num;
                }
                else if ((num > max_2) && (num != max)) {
                    max_2 = num;
                }
            }
            else if (max_flag && !max_2_flag) {
                if (num > max) {
                    max_2 = max;
                    max = num;
                    max_2_flag = 1;
                }
                else if (num < max) {
                    max_2 = num;
                    max_2_flag = 1;
                }

            }
            else if (!max_flag) {
                max = num;
                max_flag = 1;
            }
        }
    }

    if (max_2_flag) {
        printf("%.2f\n", (float)max_2 / 100.0);
    }
    return 0;
}