// =================================================================================================
//                             STR 22/23 - Pratical Assignment 1
//                                Daniela Fernandes 2019233462
//                                  Manuel Santos 2019231352
//
// compile: sudo gcc -Wall -O2 -D_REENTRANT ex3_InvRMPO.c func.o timestamps.c -o ex3_InvRMPO -lpthread -lrt
//                                       make: make ex3_InvRMPO
//                                      run: sudo ./ex3_InvRMPO
//
//  compile: sudo gcc -Wall -O2 -D_REENTRANT ex3_InvRMPO.c func2.o timestamps.c -o ex3_InvRMPO_func2 -lpthread -lrt
//                                   make: make ex3_InvRMPO_func2
//                                  run: sudo ./ex3_InvRMPO_func2
// =================================================================================================
#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>

#include "func.h"
#include "timestamps.h"

#define CLASS 1
#define GROUP 1
#define NUM_THREADS 3
#define TEST_TIME 6.0
#define DELAY 2

// Functions
const void (*func[])(int, int) = {f1, f2, f3};

// Reference Time
struct timespec zeroHour, finalHour;

// Auxiliar Variables
const int periods[] = {100.0, 200.0, 300.0}; // in milliseconds
int runs[NUM_THREADS];

// Regist all values and times of interest
struct LogTable
{
    int iteration;
    struct timespec activation, deadline;
    struct timespec begin, end;
};

struct LogTable *regists[NUM_THREADS];

//===================================================================================================
//======================================  Thread Routine  ===========================================
//===================================================================================================
void *threadRoutine(void *id)
{
    // ID of the thread
    int threadId = *(int *)id;

    // Allocate memory for regists
    regists[threadId] = (struct LogTable *)malloc(sizeof(struct LogTable) * runs[threadId]);

    struct timespec start, stop, period;
    int count = 0;

    // Set period
    period.tv_sec = 0;
    period.tv_nsec = periods[threadId] * 1e6;

    // First activation time and deadline
    regists[threadId][count].activation = zeroHour;
    regists[threadId][count].activation.tv_sec += DELAY;
    regists[threadId][count].deadline = regists[threadId][count].activation;
    regists[threadId][count].deadline = sum_timestamp(regists[threadId][count].deadline, period);

    while (1)
    {
        // Wait for activation time
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &regists[threadId][count].activation, NULL);
        // Get timestamp before computation
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        {
            perror("Error getting start time\n");
            exit(-1);
        }

        // Call function
        func[threadId](CLASS, GROUP);

        // Get timestamp after computation
        if (clock_gettime(CLOCK_MONOTONIC, &stop) == -1)
        {
            perror("Error getting stop time\n");
            exit(-1);
        }

        // Save values
        regists[threadId][count].iteration = count;
        regists[threadId][count].begin = start;
        regists[threadId][count].end = stop;

        // Update stats
        count++;

        regists[threadId][count].activation = sum_timestamp(regists[threadId][count - 1].activation, period);
        regists[threadId][count].deadline = sum_timestamp(regists[threadId][count].activation, period);

        // Check if test time is over
        if (time_between_timestamp(regists[threadId][count].activation, finalHour) < 0)
            break;
    }
    return NULL;
}

//===================================================================================================
//==========================================  Main  =================================================
//===================================================================================================
int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attributes[NUM_THREADS];
    struct sched_param schedule;

    cpu_set_t set;

    // Set CPU affinity (run the process only on CPU 0)
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
    {
        perror("Error executing sched_setaffinity\n");
        exit(-1);
    }

    // Lock all pages mapped into the address space of the calling process
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
    {
        perror("Error executing mlockall\n");
        exit(-1);
    }

    // Calculate the maximum running times of each function
    for (int i = 0; i < NUM_THREADS; i++)
        runs[i] = 1 + (TEST_TIME * 1e3 / periods[i]);

    // Get initial time
    if (clock_gettime(CLOCK_MONOTONIC, &zeroHour) == -1)
    {
        printf("Error getting initial time...\n");
        exit(-1);
    }
    // Calculate final time
    finalHour = zeroHour;
    finalHour.tv_sec += (DELAY + TEST_TIME);

    // ================================== THREADS ==================================
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // printf(" Main: creating thread %d for function %d\n", i, i + 1);

        // Initialize thread attributes
        if (pthread_attr_init(&attributes[i]) != 0)
        {
            printf("Failed intializing attributes of thread %d\n", i);
            exit(-1);
        }

        // Set inherit-scheduler attribute of thread to value specified by attributes[]
        if (pthread_attr_setinheritsched(&attributes[i], PTHREAD_EXPLICIT_SCHED) != 0)
        {
            printf("Failed attributing inherence attributes for thread %d\n", i);
            exit(-1);
        }

        // Set scheduling policy
        if (pthread_attr_setschedpolicy(&attributes[i], SCHED_FIFO) != 0)
        {
            printf("Failed setting scheduling policy for thread %d\n", i);
            exit(-1);
        }

        // Set threads priorities (works for this case)
        if ((schedule.sched_priority = sched_get_priority_min(SCHED_FIFO) + i) == -1)
        {
            printf("Failed setting priority for thread %d\n", i);
            exit(-1);
        }
        // printf("Priority of thread %d: %d\n", i, schedule.sched_priority);

        // Set schedule attributes of thread to value specified by schedule
        if (pthread_attr_setschedparam(&attributes[i], &schedule) != 0)
        {
            printf("Failed setting scheduling attribute for thread %d\n", i);
            exit(-1);
        }

        // After all atributtes are set, create the thread
        if (pthread_create(&threads[i], &attributes[i], threadRoutine, &i) != 0)
        {
            printf("Failed creating thread %d\n", i);
            exit(-1);
        }
    }

    // Wait for all threads to finnish execution
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    // Calculate and print results of execution
    double duration, jitter, biggestTime, smallestTime;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        biggestTime = 0;
        smallestTime = __DBL_MAX__;

        printf(" ============================== Thread[%d] Informations ==============================\n", i);

        for (int j = 0; j < runs[i]; j++)
        {
            // Calculate execution time
            duration = time_between_timestamp(regists[i][j].begin, regists[i][j].end);

            // Check max and min
            if (duration > biggestTime)
                biggestTime = duration;
            if (duration < smallestTime)
                smallestTime = duration;

            // Calculate missed deadline
            if (time_between_timestamp(regists[i][j].end, regists[i][j].deadline) > 0)
                printf("    Thread[%d]: Met    deadline in iteration %3d, with computation time = %3.4f ms (from %gms to %gms) \n",
                       i, j, duration, time_between_timestamp(zeroHour, regists[i][j].begin) - 2000, time_between_timestamp(zeroHour, regists[i][j].end) - 2000);
            else
                printf("    Thread[%d]: Missed deadline in iteration %3d, with computation time = %3.4f ms (from %gms to %gms) \n",
                       i, j, duration, time_between_timestamp(zeroHour, regists[i][j].begin) - 2000, time_between_timestamp(zeroHour, regists[i][j].end) - 2000);
        }
        // Calculate jitter
        jitter = biggestTime - smallestTime;

        // Show results
        printf("\n Thread[%d] Stats -->  Max = %g ms || Min = %g ms || Jitter = %g ms\n\n\n", i, biggestTime, smallestTime, jitter);

        free(regists[i]);
    }

    return 0;
}