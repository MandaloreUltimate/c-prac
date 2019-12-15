#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
    int fd[2][2];
    pipe(fd[1]);

    if (!fork()) {
        pipe(fd[0]);
        double num;
        FILE *file = fopen(argv[1], "r");
        while (fscanf(file, "%lf", &num) == 1) {
            if (num >= 0) {
                write(fd[0][1], &num, sizeof(num));
            }
        }

        if (!fork()) {
            close(fd[0][1]);
            close(fd[1][0]);
            double sum = 0;
            while (read(fd[0][0], &num, sizeof(num)) == sizeof(num)) {
                sum += num;
            }
            close(fd[0][0]);
            write(fd[1][1], &sum, sizeof(sum));
            close(fd[1][1]);
            exit(0);
        }

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                close(fd[i][j]);
            }
        }

        while (wait(NULL) > 0);
        exit(0);
    }
    close(fd[1][1]);
    while (wait(NULL) > 0);
    double ans;
    if (read(fd[1][0], &ans, sizeof(ans)) == sizeof(ans)) {
        printf("%.10g\n", ans);
    }
    close(fd[1][0]);
    return 0;
}
