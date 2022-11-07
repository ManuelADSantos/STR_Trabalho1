// sudo gcc -Wall -O2 -D_REENTRANT ex3.c func.o -o ex3 -lpthread -lrt

#define _GNU_SOURCE // must be before all includes /* To get pthread_getattr_np() declaration */
#include <stdio.h>
#include <time.h> //clock_gettime
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h> //mlockall
#include "func.h"

#define CLASS 1
#define GROUP 1

struct timespec difference(const struct timespec begin, const struct timespec end);
const void (*func[])(int, int) = {f1, f2, f3};

typedef struct // table to register all wanted times
{
    int task;
    struct timespec activation;
    struct timespec begin;
    struct timespec end;
    struct timespec deadline;
} LogTable;

LogTable entryTable[1000]; // table to register all wanted times
struct timespec zero;      // to calculate the time difference
int counter = 0;

// Rate monotonic scheduling - the higher the frequency (1/period) of a task, the higher is its priority

void *t1(void *arguments) // threads for function f1
{
    printf("\n\nIm in function T1\n\n");
    struct timespec endTime, deadlineTime, beginTime, activationTime, sleepTime;
    deadlineTime.tv_sec = 0;
    deadlineTime.tv_nsec = 0;
    struct timespec Diff;

    int priority = 30; // f1 has the highest priority according to RMPO - bigger frequency => higher priority

    struct sched_param threadParam; // holds thread parameters
    threadParam.sched_priority = priority;

    int returnValue = pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam);
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam) != 0) // SCHED_FIFO can be used only with static priorities higher than 0
    {                                                                         // pthread_self - obtain ID of the calling thread
        printf("Failed while atributing task priority \n");
        exit(1);
    }
    // debug - print thread priority to check if it was set correctly
    int test;
    struct sched_param parameter;
    if (pthread_getschedparam(pthread_self(), &test, &parameter) != 0) /* returns the scheduling policy and parameters of
                                                                        * the thread thread, in the buffers pointed to
                                                                        * by policy and param, respectively
                                                                        */
    {
        printf("Error getting priority of thread 1... \n");
        exit(1);
    }
    printf("Priority of thread 1: %d\n", parameter.sched_priority);

    while (deadlineTime.tv_sec < 6.0)
    {
        deadlineTime.tv_nsec += 1E8;

        activationTime.tv_sec = deadlineTime.tv_sec;
        activationTime.tv_nsec = deadlineTime.tv_nsec;
        deadlineTime.tv_nsec = deadlineTime.tv_nsec + 1E8;

        if (deadlineTime.tv_nsec >= 1E9)
        {
            // corrects the seconds and nanoseconds
            deadlineTime.tv_sec += 1;
            deadlineTime.tv_nsec -= 1E9;
        }

        // The CLOCK_MONOTONIC clock is not affected by discontinuous jumps in the system time

        clock_gettime(CLOCK_MONOTONIC, &beginTime); // get the task' begin time
        beginTime = difference(zero, beginTime);

        func[0](1, 1); // f1(CLASS, GROUP);

        clock_gettime(CLOCK_MONOTONIC, &endTime); // get task's end time
        Diff = difference(zero, endTime);         // Diff é a diferença entre o tempo de fim e o tempo zero
                                                  // "Diff" é o tempo que a tarefa demorou a ser executada

        sleepTime = difference(Diff, deadlineTime); // sleepTime = deadlineTime - endTime

        entryTable[counter].deadline = deadlineTime;
        entryTable[counter].activation = activationTime;
        entryTable[counter].begin = beginTime;
        entryTable[counter].end.tv_sec = Diff.tv_sec;
        entryTable[counter].end.tv_nsec = Diff.tv_nsec;
        entryTable[counter].task = 1;
        counter++;

        // allows the calling thread to sleep for an interval specified with nanosecond precision.
        if (sleepTime.tv_nsec > 0)
        {
            clock_nanosleep(CLOCK_MONOTONIC, 0, &sleepTime, NULL);
        }
    }
    printf("\nthread 1 finished. \n");
    return NULL;
}

// ========================================================================================================
// ========================================================================================================
// ========================================================================================================
// create threads for function f2
void *t2(void *arguments)
{
    printf("\n\nIm in function T2\n\n");
    struct timespec endTime, deadlineTime, beginTime, activationTime, sleepTime;
    deadlineTime.tv_sec = 0;
    deadlineTime.tv_nsec = 0;
    struct timespec Diff;

    int priority = 20; // f2 priority

    struct sched_param threadParam;        // holds thread parameters
    threadParam.sched_priority = priority; // set thread priority

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam) != 0)
    {
        printf("Failed while atributing thread priority.\n");
        exit(1);
    }
    printf("Thread 2 priority: %d \n", threadParam.sched_priority); // check if thread priority was set

    // time_between_timestamp(struct timespec begin, struct timespec end)
    while (deadlineTime.tv_sec < 6) // maintain during about 6 seconds
    {
        activationTime.tv_sec = deadlineTime.tv_sec;
        activationTime.tv_nsec = deadlineTime.tv_nsec;

        if (deadlineTime.tv_nsec >= 1E9) // check the nanoseconds
        {
            deadlineTime.tv_sec += 1;
            deadlineTime.tv_nsec -= 1E9;
        }

        clock_gettime(CLOCK_MONOTONIC, &beginTime); // get the task' begin time
        beginTime = difference(zero, beginTime);
        printf("estou %d aqui", entryTable->task);
        func[1](2, 2); // calls the function f2
        // f2(CLASS, GROUP); // call function f2

        clock_gettime(CLOCK_MONOTONIC, &endTime); // get the task' end time
        Diff = difference(zero, endTime);

        sleepTime = difference(Diff, deadlineTime); // sleepTime = deadlineTime - endTime

        entryTable[counter].deadline = deadlineTime;
        entryTable[counter].activation = activationTime;
        entryTable[counter].begin = beginTime;
        entryTable[counter].end.tv_sec = Diff.tv_sec;
        entryTable[counter].end.tv_nsec = Diff.tv_nsec;
        entryTable[counter].task = 2;
        counter++;

        if (sleepTime.tv_nsec > 0)
        {
            clock_nanosleep(CLOCK_MONOTONIC, 0, &sleepTime, NULL);
        }
    }
    printf("\nThread 2 ended.\n");
    return NULL;
}
// ========================================================================================================
// ========================================================================================================
// ========================================================================================================

void *t3(void *arguments) // threads for function f3
{
    printf("\n\nIm in function T3\n\n");
    struct timespec endTime, deadlineTime, beginTime, activationTime, sleepTime;
    deadlineTime.tv_sec = 0;
    deadlineTime.tv_nsec = 0;
    struct timespec Diff;

    int priority = 10; // f3 priority

    struct sched_param threadParam;
    threadParam.sched_priority = priority;

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam) != 0)
    {
        printf("Failed while atributing task priority \n");
        exit(1);
    }
    printf("Thread 3 priority: %d \n", threadParam.sched_priority); // check if thread priority was set

    while (deadlineTime.tv_sec < 6) // maintain during about 6 seconds
    {
        activationTime.tv_sec = deadlineTime.tv_sec;
        activationTime.tv_nsec = deadlineTime.tv_nsec; // activation time is the same as the deadline time

        if (deadlineTime.tv_nsec >= 1E9) // check the nanoseconds
        {
            deadlineTime.tv_sec += 1;
            deadlineTime.tv_nsec -= 1E9;
        }

        clock_gettime(CLOCK_MONOTONIC, &beginTime); // get the task' begin time
        beginTime = difference(zero, beginTime);

        func[2](3, 3);
        // f3(CLASS, GROUP);

        clock_gettime(CLOCK_MONOTONIC, &endTime); // get the task' end time
        Diff = difference(zero, endTime);
        sleepTime = difference(Diff, deadlineTime); // sleepTime = deadlineTime - endTime

        entryTable[counter].deadline = deadlineTime;
        entryTable[counter].activation = activationTime;
        entryTable[counter].begin = beginTime;
        entryTable[counter].end.tv_sec = Diff.tv_sec;
        entryTable[counter].end.tv_nsec = Diff.tv_nsec;
        entryTable[counter].task = 3;
        counter++;

        if (sleepTime.tv_nsec > 0) //
        {
            clock_nanosleep(CLOCK_MONOTONIC, 0, &sleepTime, NULL);
        }
    }
    printf("\nThread 3 ended.\n");
    return NULL;
}

//===============================================================================
//===============================MAIN============================================
//===============================================================================
int main(int argc, char **argv)
{
    pthread_t threads[3];
    int i = 0;
    pthread_attr_t attr1, attr2, attr3;
    int inheritsched1, inheritsched2, inheritsched3;
    struct timespec biggestTime[3], smallestTime[3], jitter[3];
    struct timespec responseTime;
    cpu_set_t set;

    clock_gettime(CLOCK_MONOTONIC, &zero); // get the time when the program starts

    // Lock all pages mapped into the address space of the calling process
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
    {
        perror("mlockall failed");
        exit(1);
    }
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    printf("mask cpu 0\n");
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1)
    {
        perror("sched_setaffinity failed");
        exit(1);
    }

    printf("main: creating thread for F1.\n");
    threads[i] = i;

    inheritsched1 = PTHREAD_EXPLICIT_SCHED; // Inherit scheduler = PTHREAD_INHERIT_SCHED

    /*
     * PTHREAD_INHERIT_SCHED:
     * Threads that are created using attr inherit scheduling attributes from
     * the creating thread; the scheduling attributes in attr are ignored.
     *
     * PTHREAD_EXPLICIT_SCHED:
     * Threads that are created using attr take
     * their scheduling attributes from the values specified by the attributes object.
     */

    if (pthread_attr_setinheritsched(&attr1, inheritsched1) != 0)
    {
        printf("Failed attributing inherence attribuits 1.\n");
        exit(1);
    }
    printf("Inherence of attribuits 1 done.\n");
    // Create thread:
    int threadReturn;
    threadReturn = pthread_create(&threads[i], NULL, t1, NULL); // On success, pthread_create() returns 0; on error, it returns an error number
    if (threadReturn != 0)
    {
        printf("\nfailed creating thread for f1.\n");
        exit(1);
    }
    printf("main: Thread 1 created.\n");
    // ============================ F2 ==============================
    i++; // increment counter i to create the next thread

    printf("\nCreating thread for F2\n");
    threads[i] = i;

    inheritsched2 = PTHREAD_EXPLICIT_SCHED;
    if (pthread_attr_setinheritsched(&attr2, inheritsched2) != 0)
    {
        printf("Failed attributing inherence attribuits 2.\n");
        exit(1);
    }
    printf("Inherence of attribuits 2 done.\n");
    threadReturn = pthread_create(&threads[i], NULL, t2, NULL);
    if (threadReturn != 0)
    {
        printf("\nfailed creating thread for f2.\n");
        exit(1);
    }
    printf("main: Thread 2 created.\n");
    // ============================ F3 ==============================
    i++;
    printf("\nCreating thread for F3\n");
    threads[i] = i;

    inheritsched3 = PTHREAD_EXPLICIT_SCHED;
    if (pthread_attr_setinheritsched(&attr3, inheritsched3) != 0)
    {
        printf("Failed attributing inherence attribuits 3.\n");
        exit(1);
    }
    printf("Inherence of attribuits 3 done.\n");
    threadReturn = pthread_create(&threads[i], NULL, t3, NULL);
    if (threadReturn != 0)
    {
        printf("\nfailed creating thread for f3.\n");
        exit(1);
    }
    printf("main: Thread 3 created.\n");
    printf("\nFinished creating all threads.\n");
    i = 0; // reset counter

    // wait for all threads to finnish execution
    while (i < 3)
    {
        threadReturn = pthread_join(threads[i], NULL);
        if (!threadReturn)
        {
            printf("thread %d not finished yet\n.", i + 1);
        }
        printf("main: thread %d finished.\n", i + 1);
        i++; // increment
    }

    for (int index = 0; index < 3; index++) // set everything to 0
    {
        biggestTime[index].tv_sec = 0;
        biggestTime[index].tv_nsec = 0;
        smallestTime[index].tv_sec = 0;
        smallestTime[index].tv_nsec = 0;
        jitter[index].tv_sec = 0;
        jitter[index].tv_nsec = 0;
    }

    int j = 0;
    while (j < counter)
    {
        // check if the deadline was met
        if (entryTable[j].end.tv_sec > entryTable[j].deadline.tv_sec)
        {
            printf("Task %d missed deadline.\n", entryTable[j].task);
        }
        else
        {
            printf("Task %d met deadline.\n", entryTable[j].task);
        }

        printf("Task %d\n", entryTable[j].task);
        printf("Activation: %ld.%ld\n", entryTable[j].activation.tv_sec, entryTable[j].activation.tv_nsec);
        printf("Deadline: %ld.%ld\n", entryTable[j].deadline.tv_sec, entryTable[j].deadline.tv_nsec);
        printf("Begin: %ld.%ld\n", entryTable[j].begin.tv_sec, entryTable[j].begin.tv_nsec);
        printf("End: %ld.%ld\n", entryTable[j].end.tv_sec, entryTable[j].end.tv_nsec);
        printf("Response time: %ld.%ld\n", responseTime.tv_sec, responseTime.tv_nsec);

        // calculate the response time
        responseTime = difference(entryTable[j].activation, entryTable[j].end);
        switch (entryTable[j].task)
        {
        case 1: // task 1
            if (responseTime.tv_sec > biggestTime[0].tv_sec)
            {
                biggestTime[0].tv_sec = responseTime.tv_sec;
                biggestTime[0].tv_nsec = responseTime.tv_nsec;
            }
            if (responseTime.tv_sec < smallestTime[0].tv_sec)
            {
                smallestTime[0].tv_sec = responseTime.tv_sec;
                smallestTime[0].tv_nsec = responseTime.tv_nsec;
            }
            break;
        case 2: // task 2
            if (responseTime.tv_sec > biggestTime[1].tv_sec)
            {
                biggestTime[1].tv_sec = responseTime.tv_sec;
                biggestTime[1].tv_nsec = responseTime.tv_nsec;
            }
            if (responseTime.tv_sec < smallestTime[1].tv_sec)
            {
                smallestTime[1].tv_sec = responseTime.tv_sec;
                smallestTime[1].tv_nsec = responseTime.tv_nsec;
            }
            break;
        case 3: // task 3
            if (responseTime.tv_sec > biggestTime[2].tv_sec)
            {
                biggestTime[2].tv_sec = responseTime.tv_sec;
                biggestTime[2].tv_nsec = responseTime.tv_nsec;
            }
            if (responseTime.tv_sec < smallestTime[2].tv_sec)
            {
                smallestTime[2].tv_sec = responseTime.tv_sec;
                smallestTime[2].tv_nsec = responseTime.tv_nsec;
            }
            break;
        default:
            printf("Error: task not found.\n");
            break;
            j++;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        jitter[i] = difference(smallestTime[i], biggestTime[i]); // calculate the jitter
        printf("Jitter: %ld.%ld\n", jitter[i].tv_sec, jitter[i].tv_nsec);
    }
    return 0;
}
// ==========================================================
struct timespec difference(const struct timespec begin, const struct timespec end)
{
    struct timespec calc;

    calc.tv_sec = end.tv_sec - begin.tv_sec;
    calc.tv_nsec = end.tv_nsec - begin.tv_nsec;

    if (calc.tv_nsec < 0)
    {
        calc.tv_sec -= 1;
        calc.tv_nsec += 1E9;
    }
    return calc;
}
