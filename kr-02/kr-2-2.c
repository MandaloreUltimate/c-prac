#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>

int count_bits(long long n)
{
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

int main(int argc, char *argv[])
{
    pid_t *pids = (pid_t*) calloc(argc + 1, sizeof(*pids));
    int *vals = (int*) calloc(argc + 1, sizeof(*vals));

    pid_t pid;
    long long num;
    int bitcount;
    int status;
    int cur_pid;

    for (int i = 1; i < argc; i++) {
        if (!(pid = fork())) {
            sscanf(argv[i], "%lld", &num);
            bitcount = count_bits(num);
            exit(bitcount);
        } else {
            pids[i] = pid;
        }
    }
    for (int i = 1; i < argc; i++) {
        cur_pid = wait(&status);
        for (int j = 1; j < argc; j++) {
            if (cur_pid == pids[j]) {
                vals[j] = WEXITSTATUS(status);
            }
        }
    }
    for (int i = 1; i < argc; i++) {
        printf("%d\n", vals[i]);
    }
    return 0;
}
