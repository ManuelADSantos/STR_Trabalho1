// gcc -Wall -O2 -D_REENTRANT func2.c -c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "timestamps.h"

void f1(int a, int b)
{
    int count = 0;
    struct timespec begin, now;
    if (clock_gettime(CLOCK_MONOTONIC, &begin) == -1)
    {
        printf("Error obtaining f1 begin time\n");
        exit(-1);
    }

    while (1)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        {
            printf("Error obtaining f1 now time\n");
            exit(-1);
        }
        if (time_between_timestamp(begin, now) >= 36.001127)
            return;

        // 30 + ()%9

        count++;
    }
}

void f2(int a, int b)
{
    int count = 0;
    struct timespec begin, now;
    if (clock_gettime(CLOCK_MONOTONIC, &begin) == -1)
    {
        printf("Error obtaining f2 begin time\n");
        exit(-1);
    }

    while (1)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        {
            printf("Error obtaining f2 now time\n");
            exit(-1);
        }
        if (time_between_timestamp(begin, now) >= 56.001693)
            return;

        count++;
    }
}

void f3(int a, int b)
{
    int count = 0;
    struct timespec begin, now;
    if (clock_gettime(CLOCK_MONOTONIC, &begin) == -1)
    {
        printf("Error obtaining f3 begin time\n");
        exit(-1);
    }

    while (1)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        {
            printf("Error obtaining f3 now time\n");
            exit(-1);
        }
        if (time_between_timestamp(begin, now) >= 86.002828)
            return;

        count++;
    }
}