#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

int main(void)
{
    int_least32_t n;
    int width;
    scanf("%"SCNo32 "%o", &n, &width);

    int_least32_t rows_cnt = 1 << n;
    int_least32_t k = 0;

    for (int_least32_t i = 0; i < rows_cnt; i++) {
        printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", width, i, width, i, width, (rows_cnt / 2 > i) ? i : -k++);
    }
    return 0;
}