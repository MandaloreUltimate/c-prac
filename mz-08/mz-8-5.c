#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    int num, status;
    pid_t father = getpid();
    while (scanf("%d", &num) == 1) {
        pid_t pid;
        if ((pid = fork()) < 0) {
            exit(1);
        } else if (pid > 0) {
            while (waitpid(pid, &status, 0) == -1);
            if (!WEXITSTATUS(status)) {
                printf("%d\n", num);
                exit(0);
            } else {
                if (getpid() != father) {
                    exit(1);
                } else {
                    printf("-1\n");
                    exit(0);
                }
            }
        }
    }
}