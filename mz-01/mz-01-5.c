#include <stdio.h>
#include <stdlib.h>

enum { MAX_SIZE = 2000 };

int
is_prime(int num)
{
    if (num <= 1) {
        return 0;
    }
    if (num % 2 == 0 && num > 2) {
        return 0;
    }
    for(int i = 3; i < num / 2; i+= 2) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(void)
{
    int n, i, j;
    if (scanf("%d", &n) != 1 || n > MAX_SIZE || !is_prime(n)) {
        return 1;
    }
    int *table = (int *) calloc(MAX_SIZE, sizeof(*table));
    if(!table) {
        return 1;
    }
    for (i = 0; i < n; i++) {
        int k = 1;
        while (k < n && (k * (i + 1)) % n != 1) {
            k++;
        }
        table[i] = k;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n - 1; j++) {
            printf("%d ", (i * table[j]) % n);
        }
        printf("\n");
    }
    free(table);
    return 0;
}