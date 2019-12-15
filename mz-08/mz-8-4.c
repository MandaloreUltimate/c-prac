#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int
main(void)
{
    int n;
    scanf("%d", &n);
    setbuf(stdout, NULL);

    for (int i = 1; i < n + 1; i++) {
        pid_t pid;
        if ((pid = fork()) < 0) {
            printf("fork: %s\n", strerror(errno));
        } else if (pid == 0) {
            if (i != n) {
                printf("%d ", i);
            } else {
                printf("%d\n", i);
                exit(0);
            }
        } else if (pid > 0) {
            int status;
            while (waitpid(pid, &status, 0) == -1);
            exit(0);
        }
    }
    return 0;
}
