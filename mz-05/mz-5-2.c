#include <stdio.h>
#include <stdlib.h>

enum { BASE = 8, };

int
main(int argc, char *argv[])
{
    const char tmp[] = "xwrxwrxwr";
    int len = sizeof(tmp) - 1;
    char *end, ch;
    unsigned long rights;
    for (int i = 1; i < argc; i++) {
        rights = strtoul(argv[i], &end, BASE);
        for (int j = len - 1; j >= 0; j--) {
            ch = (1 << j) & rights ? tmp[j] : '-';
            printf("%c", ch);
        }
        printf("\n");
    }
    return 0;
}
