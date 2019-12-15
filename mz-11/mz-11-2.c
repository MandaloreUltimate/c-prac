#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

enum
{
    MULT = 0,
    ADD = 1
};

volatile sig_atomic_t flag = ADD;

void
SigHndlr(int sig)
{
    if (sig == SIGINT) {
        signal(SIGINT, SigHndlr);
        flag = ADD;
    } else if (sig == SIGQUIT) {
        signal(SIGQUIT, SigHndlr);
        flag = MULT;
    }
}

int
main(void)
{
    signal(SIGINT, SigHndlr);
    signal(SIGQUIT, SigHndlr);

    pid_t pid = getpid();
    printf("%d\n", pid);
    fflush(stdout);

    int num, ans = 0;

    while (scanf("%d", &num) == 1) {
        if (flag == ADD) {
            __builtin_add_overflow(ans, num, &ans);
            printf("%d\n", ans);
            fflush(stdout);
        } else if (flag == MULT) {
            __builtin_mul_overflow(ans, num, &ans);
            printf("%d\n", ans);
            fflush(stdout);
        }
    }
    return 0;
}