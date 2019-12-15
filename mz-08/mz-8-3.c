#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

enum
{
    BUF_SIZE = 9,
    DEC_BASE = 10
};

void
proc(int n)
{
    char buf[BUF_SIZE];
    read(STDIN_FILENO, buf, sizeof(buf) - 1);
    long num = strtol(buf, NULL, DEC_BASE);
    printf("%d %ld\n", n, num * num);
}

int
main(void)
{
    pid_t p1, p2, p3;
    if ((p1 = fork()) > 0) {
        if ((p2 = fork()) > 0) {
            if ((p3 = fork()) > 0) {
                int status;
                while(wait(&status) != -1);
            } else if (!p3) {
                proc(3);
            }
        } else if (!p2) {
            proc(2);
        }
    } else if (!p1) {
        proc(1);
    }
    return 0;
}