#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int
proc(const char *cmd)
{
    pid_t pid = fork();
    if (pid == 0) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return ((WIFEXITED(status)) && !WEXITSTATUS(status));
    }
}

int
main(int argc, char *argv[])
{
    return !((proc(argv[1]) || proc(argv[2])) && proc(argv[3]));
}