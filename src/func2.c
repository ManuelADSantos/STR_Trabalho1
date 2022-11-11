// =================================================================================================
//                             STR 22/23 - Pratical Assignment 1
//                                Daniela Fernandes 2019233462
//                                  Manuel Santos 2019231352
//
//                      compile: gcc -Wall -O2 -D_REENTRANT func2.c -c
// =================================================================================================
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "timestamps.h"

void f1(int a, int b)
{
    struct timespec begin, now;
    double aux = 0, passed = 0, end = 36.027659;

    if (clock_gettime(CLOCK_MONOTONIC, &begin) == -1)
    {
        printf("Error obtaining f1 begin time\n");
        exit(-1);
    }

    while (passed < end)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        {
            printf("Error obtaining f1 now time\n");
            exit(-1);
        }

        aux = time_between_timestamp(begin, now);
        passed += aux;
        begin = now;
    }
}

void f2(int a, int b)
{
    struct timespec begin, now;
    double aux = 0, passed = 0, end = 56.074312;

    if (clock_gettime(CLOCK_MONOTONIC, &begin) == -1)
    {
        printf("Error obtaining f2 begin time\n");
        exit(-1);
    }

    while (passed < end)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        {
            printf("Error obtaining f2 now time\n");
            exit(-1);
        }

        aux = time_between_timestamp(begin, now);
        passed += aux;
        begin = now;
    }
}

void f3(int a, int b)
{
    struct timespec begin, now;
    double aux = 0, passed = 0, end = 86.061494;

    if (clock_gettime(CLOCK_MONOTONIC, &begin) == -1)
    {
        printf("Error obtaining f3 begin time\n");
        exit(-1);
    }

    while (passed < end)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        {
            printf("Error obtaining f3 now time\n");
            exit(-1);
        }

        aux = time_between_timestamp(begin, now);
        passed += aux;
        begin = now;
    }
}