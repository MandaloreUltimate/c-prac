#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    if (fork() == 0) {
        if(fork() == 0) {
            printf("3 ");
            exit(0);
        }
        while (wait(NULL) != -1);
        printf("2 ");
        exit(0);
    }
    while (wait(NULL) != -1);
    printf("1\n");
    return 0;
}