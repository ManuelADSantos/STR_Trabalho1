// =================================================================================================
//                             STR 22/23 - Pratical Assignment 1
//                                Daniela Fernandes 2019233462
//                                  Manuel Santos 2019231352
//
//      compile: sudo gcc -Wall -O2 -D_REENTRANT ex1.c func.o timestamps.c -o ex1 -lpthread -lrt
//                                       make: make ex1
//                                      run: sudo ./ex1
//
//  compile: sudo gcc -Wall -O2 -D_REENTRANT ex1.c func2.o timestamps.c -o ex1_func2 -lpthread -lrt
//                                    make: make ex1_func2
//                                   run: sudo ./ex1_func2
// =================================================================================================
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>

#include "func.h"
#include "timestamps.h"

#define TEST_ITERATIONS 50
#define NUM_THREADS 3
#define CLASS 1
#define GROUP 1

// Functions
const void (*func[])(int, int) = {f1, f2, f3};

// =================================================================================================
//                                              MAIN
// =================================================================================================
int main(int argc, char **argv)
{
    struct timespec start, stop;
    double calc = 0.0;
    double times[3][2] = {{__DBL_MAX__, 0.0}, {__DBL_MAX__, 0.0}, {__DBL_MAX__, 0.0}};
    cpu_set_t mask;

    // Set CPU affinity (run the process only on CPU 0)
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (sched_setaffinity(getpid(), sizeof(mask), &mask) == -1)
    {
        perror("sched_setaffinity failed");
        exit(-1);
    }

    printf("\n");

    // Lock all pages mapped into the address space of the calling process
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
    {
        perror("mlockall failed");
        exit(-1);
    }

    // Measure computation times
    for (int j = 0; j < NUM_THREADS; j++)
    {
        for (int i = 0; i < TEST_ITERATIONS; i++)
        {
            // Get timestamp before computation
            if (clock_gettime(CLOCK_REALTIME, &start) == -1)
            {
                perror("clock gettime");
                exit(-1);
            }

            func[j](CLASS, GROUP);

            // Get timestamp after computation
            if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
            {
                perror("clock gettime");
                exit(-1);
            }

            // Calculate time between timestamps
            calc = time_between_timestamp(start, stop);
            // Record minimum and maximum times
            if (calc < times[j][0])
            {
                times[j][0] = calc;
                printf("Novo min (func1) = %lf ms (iteracao %d)\n", calc, i);
            }
            if (calc > times[j][1])
            {
                times[j][1] = calc;
                printf("Novo MAX (func1) = %lf ms (iteracao %d)\n", calc, i);
            }
        }
        printf("\n");
    }

    // Print results
    printf(" === Tempos Minimos ===\n    f1: %lf ms\n    f2: %lf ms\n    f3: %lf ms\n\n",
           times[0][0], times[1][0], times[2][0]);
    printf(" === Tempos Maximos ===\n    f1: %lf ms\n    f2: %lf ms\n    f3: %lf ms\n\n",
           times[0][1], times[1][1], times[2][1]);

    return (0);
}
