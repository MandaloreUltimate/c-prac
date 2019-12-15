#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <math.h>

enum { MAX_SIG_CNT = 3 };

volatile sig_atomic_t cur_prime = 0;
volatile sig_atomic_t sig_cnt = 0;

void
SigHndlr1(int sig)
{
    signal(SIGINT, SigHndlr1);
    if (sig_cnt < MAX_SIG_CNT) {
        printf("%d\n", cur_prime);
        fflush(stdout);
        sig_cnt++;
    } else {
        exit(0);
    }
}

void SigHndlr2(int sig)
{
    exit(0);
}

int
main(void)
{
    signal(SIGINT, SigHndlr1);
    signal(SIGTERM, SigHndlr2);

    int low, high;

    scanf("%d", &low);
    scanf("%d", &high);
    pid_t pid = getpid();
    printf("%d\n", pid);
    fflush(stdout);

    int flag_prime = 0;
    for (int i = low; i < high; i++) {
        flag_prime = 1;
        for (int j = 2; j <= sqrt(i); j++) {
            if (!(i % j)) {
                flag_prime = 0;
                break;
            }
        }
        if (flag_prime) {
            cur_prime = i;
        }
    }

    printf("-1\n");
    fflush(stdout);
    return 0;
}
