#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

enum { SIG_CONST = 128 };

int
mysys(const char *str) {
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status)) {
            return WTERMSIG(status) + SIG_CONST;
        } else {
            return WEXITSTATUS(status);
        }
    }
}