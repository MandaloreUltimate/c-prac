#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>

void
fork_tree(int i, int cur_depth, int max_depth)
{
    if (!fork()) {
        if (cur_depth < max_depth) {
            fork_tree(2 * i + 1, cur_depth + 1, max_depth);
        }
        printf("%d\n", i);
        fflush(stdout);
        if (cur_depth < max_depth) {
            fork_tree(2 * i + 2, cur_depth + 1, max_depth);
        }
        exit(0);
    } else {
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    int depth;
    sscanf(argv[1], "%d", &depth);
    fork_tree(0, 0, depth);
    return 0;
}