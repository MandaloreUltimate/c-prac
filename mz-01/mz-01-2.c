#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int ch = 'a';
    long long len = 0;
    long long max_len = 0;
    int q_flag = 0;
    do {
        ch = getchar();
        if (ch >= 'a' && ch <= 'z') {
            len++;
            if (ch == 'q') {
                q_flag = 1;
            }
        } else {
            if (max_len < len && q_flag == 0) {
                max_len = len;
            }
            q_flag = 0;
            len = 0;
        }
    } while (ch != EOF);
    printf("%lld\n", max_len);
    return 0;
}