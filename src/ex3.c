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

// Functions
const void (*func[])(int, int) = {f1, f2, f3};

// Reference Time
struct timespec zeroHour, finalHour;

// Auxiliar Variables
struct timespec biggestTime[NUM_THREADS], smallestTime[NUM_THREADS];
double jitter[NUM_THREADS];
const int periods[] = {100.0, 200.0, 300.0}; // in milliseconds
int runs[NUM_THREADS];

// Regist all values and times of interest
struct LogTable
{
    int function;
    int iteration;
    struct timespec activation;
    struct timespec begin;
    struct timespec end;
    struct timespec deadline;
};

struct LogTable *regists[NUM_THREADS];

//===================================================================================================
//======================================  Thread Routine  ===========================================
//===================================================================================================
void *threadRoutine(void *id)
{
    int threadId = *(int *)id;
    // printf("Im in threadRoutine of thread number %d\n", threadId);

    // Initialize general stats
    biggestTime[threadId].tv_sec = 0;
    biggestTime[threadId].tv_nsec = 0;
    smallestTime[threadId].tv_sec = 3600;
    smallestTime[threadId].tv_nsec = 1e10;
    jitter[threadId] = 0;
    jitter[threadId] = 0;

    // Allocate memory for regists
    regists[threadId] = (struct LogTable *)malloc(sizeof(struct LogTable) * runs[threadId]);

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

    // Get initial time
    if (clock_gettime(CLOCK_MONOTONIC, &zeroHour) == -1)
    {
        printf("Error getting initial time...\n");
        exit(-1);
    }

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

    // // Setting all values of registers
    // for (int index = 0; index < NUM_THREADS; index++) // set everything to 0
    // {
    //     biggestTime[index].tv_sec = 0;
    //     biggestTime[index].tv_nsec = 0;
    //     smallestTime[index].tv_sec = 3600;
    //     smallestTime[index].tv_nsec = 1e10;
    //     jitter[index] = 0;
    //     jitter[index] = 0;
    // }

    // Calculate the maximum running times of each function
    for (int i = 0; i < NUM_THREADS; i++)
    {
        runs[i] = 1 + (TEST_TIME * 1e3 / periods[i]);
    }

    // ================================== THREADS ==================================
    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf(" Main: creating thread %d for function %d\n", i, i + 1);

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
        if ((schedule.sched_priority = sched_get_priority_max(SCHED_FIFO) - i) == -1)
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

    // Print results of execution
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < runs[i]; j++)
        {
            //     printf("Task %d: activation time: %ld.%09ld, deadline: %ld.%09ld, begin: %ld.%09ld, end: %ld.%09ld )");
            // }
            // jitter[i] = time_between_timestamp(smallestTime[i], biggestTime[i]); // calculate the jitter
            // printf("Jitter: %g\n", jitter[i]);
        }
        free(regists[i]);
    }

    return 0;
}