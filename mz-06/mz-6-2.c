#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum
{
    START_YEAR = 1900,
    DAY_ONE = 7,
    MON_ONE = 10,
    YEAR_ONE = 1925,
    SEC_DAY = 86400,
    DAY_MON = 30,
    MON_YEAR = 12,
    SEC_MON = SEC_DAY * DAY_MON,
    SEC_YEAR = SEC_MON * MON_YEAR,
    DAY_YEAR = DAY_MON * MON_YEAR
};

int
main(void)
{
    int year, mon, day;
    struct tm start_time = {0};
    struct tm tmp = {0};
    long long new_time, new_mday, new_year, new_mon;

    start_time.tm_mday = DAY_ONE;
    start_time.tm_mon = MON_ONE - 1;
    start_time.tm_year = YEAR_ONE - START_YEAR;
    start_time.tm_isdst = -1;
    start_time.tm_hour = 0;

    while (scanf("%d %d %d", &year, &mon, &day) == 3) {
        tmp.tm_mday = day;
        tmp.tm_mon = mon - 1;
        tmp.tm_year = year - START_YEAR;
        tmp.tm_isdst = -1;
        tmp.tm_hour = 0;

        new_time = (long long) difftime(mktime(&tmp), mktime(&start_time)) + SEC_DAY / 2;
        new_year = new_time / SEC_YEAR;
        new_mon = (new_time % SEC_YEAR) / SEC_MON;
        new_mday = (new_time % SEC_MON) / SEC_DAY;

        printf("%lld %lld %lld\n", new_year + 1, new_mon + 1, new_mday + 1);
    }
    return 0;
}