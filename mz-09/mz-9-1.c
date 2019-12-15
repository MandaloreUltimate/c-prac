#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

enum
{
    ERR_CODE = 42,
    MODE = 0660
};

int
main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
        return 0;
    } else if (pid == 0) {
        int file1 = open(argv[2], O_RDONLY);
        int file2 = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, MODE);
        int file3 = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, MODE);
        if (file1 < 0 || file2 < 0 || file3 < 0) {
            _exit(ERR_CODE);
        }
        if (dup2(file1, STDIN_FILENO) < 0 || dup2(file2, STDOUT_FILENO) < 0 ||dup2(file3, STDERR_FILENO) < 0) {
            _exit(ERR_CODE);
        }
        execlp(argv[1], argv[1], NULL);
        _exit(ERR_CODE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("%d\n", status);
    }
    return 0;
}
