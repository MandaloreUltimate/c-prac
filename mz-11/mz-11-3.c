#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <limits.h>

void
proc(const char *cmd)
{
    FILE *file = fopen(cmd, "r");
    char str[PATH_MAX + 1];
    fgets(str, sizeof(str), file);
    str[strlen(str) - 1] = '\0';
    fclose(file);
    if (!fork()) {
        execlp(str, str, NULL);
        _exit(1);
    }
}

int
main(int argc, char *argv[])
{
    int status, n;
    int finished = 0;
    sscanf(argv[1], "%d", &n);

    if (n > argc - 2) {
        n = argc - 2;
    }

    for (int i = 0; i < n; i++) {
        proc(argv[i + 2]);
    }

    for (int i = 0; i < n; i++) {
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            finished++;
        }
    }

    for (int i = n + 2; i < argc; i++) {
        proc(argv[i]);
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            finished++;
        }
    }

    printf("%d\n", finished);
    return 0;
}
