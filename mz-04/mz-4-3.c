#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int
tree_search(int fd, off_t offset)
{
    struct Node node;

    if (lseek(fd, offset * sizeof(node), SEEK_SET) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }

    if (read(fd, &node, sizeof(node)) != sizeof(node)) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }

    if (node.right_idx) {
        tree_search(fd, node.right_idx);
    }

    printf("%d\n", node.key);

    if (node.left_idx) {
        tree_search(fd, node.left_idx);
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    if (tree_search(fd, 0) < 0) {
        fprintf(stderr, "Cannot traverse the tree\n");
        return 1;
    }

    return 0;
}
