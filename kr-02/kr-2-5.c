#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

enum { MODE = 0640 };

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if (!fork()) {
        dup2(fd[1], 1);
        close(fd[0]); close(fd[1]);

        if (!fork()) {
            int f4 = open(argv[4], O_RDONLY);
            dup2(f4, 0);
            close(f4);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        wait(NULL);
        exit(0);
    }

    if (!fork()) {
        int f5 = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, MODE);
        dup2(fd[0], 0);
        dup2(f5, 1);
        close(f5);
        close(fd[0]); close(fd[1]);

        if (fork() > 0) {
            int status;
            wait(&status);
            if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                execlp(argv[3], argv[3], NULL);
                _exit(1);
            }
        } else {
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
    }
    return 0;
}
