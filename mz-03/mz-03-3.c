#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

enum
{
    ACCURACY = 10000,
    PERCENT = 100,
    MAX_COURSE_VAL = 10000,
    MAX_CHANGE_ABS = 100
};

int
main(int argc, char *argv[])
{
    double course, change;

    if (sscanf(argv[1], "%lf", &course) != 1 || argc < 2) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    if (course <= 0 || course > MAX_COURSE_VAL) {
        printf("Wrong initial course value\n");
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        if (sscanf(argv[i], "%lf", &change) != 1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }

        if (change <= -MAX_CHANGE_ABS || change >= MAX_CHANGE_ABS) {
            fprintf(stderr, "Wrong change value\n");
            return 1;
        }

        course *= 1.0 + change / PERCENT;
        course = round(course * ACCURACY) / ACCURACY;
    }

    printf("%.4lf\n", course);
    return 0;
}