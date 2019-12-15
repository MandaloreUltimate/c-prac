#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>

volatile sig_atomic_t max;
FILE *f_in, *f_out;
pid_t pid[3];

void
SigHndlr(int sig)
{
    signal(SIGUSR1, SigHndlr);

    if (((pid[1]) || (pid[2]))) {
        int num;
        fscanf(f_in, "%d", &num);
        if (num < max) {
            if (pid[1]) {
                printf("%d %d\n", 1 , num);
            } else {
                printf("%d %d\n", 2 , num);
            }
            fflush(stdout);
        }
        fprintf(f_out, "%d ", num + 1);
        fflush(f_out);
        if (pid[1]) {
            kill(pid[1], SIGUSR1);
        } else {
            kill(pid[2], SIGUSR1);
        }
        if (num >= max){
            exit(0);
        }
    } else {
        fscanf(f_in, "%d", &pid[2]);
        fprintf(f_out, "1 ");
        fflush(f_out);
        kill(pid[2], SIGUSR1);
    }
 //   return;
}

void
fd_close (int fd[])
{
    for (int i = 0; i < 2; i++) {
        close(fd[i]);
    }
}

int
main(int argc, char *argv[])
{
    signal(SIGUSR1, SigHndlr);
    sscanf(argv[1], "%d", &max);
    int fd[2];
    pipe(fd);
    f_in = fdopen(fd[0], "r");
    f_out = fdopen(fd[1], "w");

    for (int i = 1; i <= 2; i++) {
        if (!(pid[i] = fork())) {
            while (1) {
                pause();
            }
        }
    }

    fprintf(f_out, "%d ", pid[2]);
    fflush(f_out);
    kill(pid[1], SIGUSR1);

    fclose(f_in);
    fclose(f_out);
    fd_close(fd);
    while (wait(NULL) > 0);
    printf("Done\n");
    fflush(stdout);
    return 0;
}