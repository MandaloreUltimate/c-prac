#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char mask[] = "rwxrwxrwx";
enum { MASK_LEN = sizeof(mask) - 1 };

int
parse_rwx_permissions(const char *str)
{
    int rights = 0;
    if (!str || strlen(str) != MASK_LEN) {
        return -1;
    }

    for (int i = 0; i < MASK_LEN; i++) {
        if (str[i] == mask[i]) {
            rights |= (1 << (MASK_LEN - 1 - i));
        } else if (str[i] != '-') {
            return -1;
        }
    }

    return rights;
}