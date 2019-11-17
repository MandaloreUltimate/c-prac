#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
main(void)
{
    long long a, b;
    scanf("%lld %lld", &a, &b);
    long long rem = a > 0 ? a % b : (a % b + b) % b;
    long long min = a - rem;
    long long max = min + b;
    long long ans = INT_MAX >= max && llabs(max - a) <= llabs(a - min) ? max : min;
    if (ans < INT_MIN) {
        ans = max;
    }
    printf("%lld\n", ans);
    return 0;
}