#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sig_cnt = 0;

enum { SIG_MAX_CNT = 5 };

void
SigHndl(int sig)
{
    signal(SIGHUP, SigHndl);
    if (sig_cnt < SIG_MAX_CNT) {
        printf("%d\n", sig_cnt);
        sig_cnt++;
        fflush(stdout);
    } else {
        exit(0);
    }
}

int
main(void)
{
    signal(SIGHUP, SigHndl);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 0;
}