#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int
check_status(int status)
{
    return (!WIFEXITED(status) || WEXITSTATUS(status));
}

int
main(int argc, char *argv[])
{
    int start = 2;
    pid_t pid;

    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[1], argv[i])) {
            if ((pid = fork()) < 0) {
                perror("Fork failed: ");
                return 1;
            } else if (!pid) {
                argv[i] = NULL;
                execvp(argv[start], &argv[start]);
                _exit(1);
            } else if (pid > 0) {
                int status;
                wait(&status);
                if (check_status(status)) {
                    perror("Command execution failed: ");
                    return 1;
                }
            }
            if (i + 1 < argc) {
                start = i + 1;
            } else {
                return 0;
            }
        }
    }

    if (argc > 2) {
        for (int i = start; i < argc; i++) {
            argv[i - 1] = argv[i];
        }
        argv[argc - 1] = NULL;

        if ((pid = fork()) < 0) {
            perror("Fork failed: ");
            return 1;
        } else if (!pid) {
            execvp(argv[start - 1], &argv[start - 1]);
            _exit(1);
        } else if (pid > 0) {
            int status;
            wait(&status);
            if (check_status(status)) {
                perror("Command execution failed: ");
                return 1;
            }
        }
    }
    return 0;
}