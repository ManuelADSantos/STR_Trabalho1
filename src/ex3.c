#define _GNU_SOURCE // must be before all includes /* To get pthread_getattr_np() declaration */
#include <stdio.h>
#include <time.h> //clock_gettime
#include <sched.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "func.h"

#define CLASS 1
#define GROUP 1

struct timespec timeStructZero;
const void (*func[])(int, int) = {f1, f2, f3};
typedef struct // table to register all wanted times
{
    int task;
    struct timespec activation;
    struct timespec begin;
    struct timespec end;
    struct timespec deadline;
} LogTable;

int counter = 0;
LogTable entryTable[20];

struct timespec sub_timestamp(struct timespec begin, struct timespec end)
{
    struct timespec result;
    if (end.tv_nsec <= begin.tv_nsec)
    {
        // Caso edge
        result.tv_sec = end.tv_sec - begin.tv_sec - 1.0;
        result.tv_nsec = (1000000000.0 - begin.tv_nsec) + end.tv_nsec;
    }
    else
    {
        result.tv_sec = end.tv_sec - begin.tv_sec;
        result.tv_nsec = end.tv_nsec - begin.tv_nsec;
    }

    return result;
}
// somar dois timestamps
struct timespec sumTimes(struct timespec *a, struct timespec *b)
{
    struct timespec calc;
    calc.tv_nsec = a->tv_nsec + b->tv_nsec;
    calc.tv_sec = a->tv_sec + b->tv_sec;
    if (calc.tv_nsec > (long int)1e9 - 1)
    {
        calc.tv_sec++;
        calc.tv_nsec = calc.tv_nsec % (long int)1e9; // resto da divisão inteira
    }
    return calc;
}

// Rate monotonic scheduling - the higher the frequency (1/period) of a task, the higher is its priority

void *t1(void *arguments) // thread for function f1
{
    struct timespec endTime, deadlineTime, beginTime, activationTime, sleepTime;
    deadlineTime.tv_sec = 0;
    deadlineTime.tv_nsec = 0;

    long sDiff;
    long nDiff;

    int priority = 50; // f1 has the highest priority according to RMPO

    struct sched_param threadParam; // holds thread parameters
    threadParam.sched_priority = priority;
    // pthread_self - obtain ID of the calling thread
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam) != 0) // SCHED_FIFO can be used only with static priorities higher than 0

    /*
     * int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
     *
     * This function sets the scheduling policy and parameters of the thread.
     * Policy specifies the new scheduling policy for thread.
     */

    {
        printf("Failed while atributing task priority \n");
        exit(1);
    }
    // debug
    int policy;
    struct sched_param parameter;
    if (pthread_getschedparam(pthread_self(), &policy, &parameter) != 0) /* returns the scheduling policy and parameters of
                                                                          * the thread thread, in the buffers pointed to
                                                                          * by policy and param, respectively
                                                                          */
    {
        printf("Error getting priority... \n");
        exit(1);
    }
    printf("Priority of thread 1: %d\n", parameter.sched_priority);

    while (deadlineTime.tv_sec < 6)
    {
        deadlineTime.tv_nsec += 1E8;

        activationTime.tv_sec = deadlineTime.tv_sec;
        activationTime.tv_nsec = deadlineTime.tv_nsec;
        deadlineTime.tv_nsec = deadlineTime.tv_nsec + 1E8;

        if (deadlineTime.tv_nsec >= 1E9)
        {
            // corrects the seconds and nanoseconds
            deadlineTime.tv_sec += 1; // add 1 second
            deadlineTime.tv_nsec -= 1E9;
        }

        clock_gettime(CLOCK_MONOTONIC, &beginTime); // get the task' begin time

        beginTime.tv_sec = beginTime.tv_sec - timeStructZero.tv_sec; // vamos preparar esse tempo para o apontar na tabela
        beginTime.tv_nsec = beginTime.tv_nsec - timeStructZero.tv_nsec;

        if (beginTime.tv_nsec < 0)
        {                          // corrects the seconds and nanoseconds if the nanoseconds reset to zero
            beginTime.tv_sec -= 1; // IMPORTANT: this code only works if the time diff is less than a second!!!
            beginTime.tv_nsec += 1E9;
        }

        f1(CLASS, GROUP);

        clock_gettime(CLOCK_MONOTONIC, &endTime);       // vamos buscar o tempo de fim da tarefa
        sDiff = endTime.tv_sec - timeStructZero.tv_sec; // vamos preparar esse tempo para o apontar na tabela
        nDiff = endTime.tv_nsec - timeStructZero.tv_nsec;

        if (nDiff < 0)
        {               // corrects the seconds and nanoseconds if the nanoseconds reset to zero
            sDiff -= 1; // IMPORTANT: this code only works if the time diff is less than a second!!!
            nDiff += 1E9;
        }

        sleepTime.tv_sec = deadlineTime.tv_sec - sDiff;
        sleepTime.tv_nsec = deadlineTime.tv_nsec - nDiff;

        if (sleepTime.tv_nsec < 0)
        {                          // corrects the seconds and nanoseconds if the nanoseconds reset to zero
            sleepTime.tv_sec -= 1; // IMPORTANT: this code only works if the time diff is less than a second!!!
            sleepTime.tv_nsec += 1E9;
        }

        threadParam.sched_priority = 70;
        if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam) != 0)
        {
            printf("Erro ao atribuir prioridade a tarefa \n");
            exit(1);
        }

        entryTable[counter].deadline = deadlineTime;
        entryTable[counter].activation = activationTime;
        entryTable[counter].begin = beginTime;
        entryTable[counter].end.tv_sec = sDiff;
        entryTable[counter].end.tv_nsec = nDiff;
        entryTable[counter].task = 1;
        counter++;

        threadParam.sched_priority = priority;
        if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &threadParam) != 0)
        {
            printf("Erro ao atribuir prioridade a tarefa \n");
            exit(1);
        }

        if (sleepTime.tv_nsec > 0)
        {
            clock_nanosleep(CLOCK_MONOTONIC, 0, &sleepTime, NULL);
        }

        // printf ("A thread 1 demorou %lds:%ldus\n", sDiff, nDiff/1000);
    }
    printf("THREAD 1: Ended. \n");
    return NULL;
}

int main(int argc, char **argv)
{
    struct timespec biggestTime[3], smallestTime[3], jitter[3], responseTime;

    pthread_t threads[3];
    int i, code = 0;

    printf("\nPractical assigment 1 Exercise number 3\n");

    // only use one core
    cpu_set_t CPUmask;    // bitset where each bit represents a CPU
    CPU_ZERO(&CPUmask);   // Clears set, so that it contains no CPUs
    CPU_SET(0, &CPUmask); // Add CPU to set

    /*  int sched_setaffinity(pid_t pid, size_t cpusetsize, const cpu_set_t *mask);
     *  -> A thread's CPU affinity mask determines the set of CPUs on which it is eligible to run.
     */

    if (sched_setaffinity(0, sizeof(cpu_set_t), &CPUmask) == -1)
    {
        printf("Failed in sched_setaffinity\n");
        exit(1);
    }

    // Main - tem a maior prioridade

    struct sched_param ThreadParam; // variable for thread parameters
    ThreadParam.sched_priority = 60;

    /*
     * pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
     * -> Sets the scheduling policy and parameters of the thread
     */

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &ThreadParam) != 0)
    {
        printf("Failed on pthread_setschedparam.\n");
        exit(1);
    }

    // Create first Thread
    printf("Main: thread for F1.\n");
    threads[i] = i;
    pthread_attr_t attr1;

    /*
     * The pthread_attr_init() function initializes the thread
     * attributes object pointed to by attr with default attribute
     * values
     */

    int sched1 = PTHREAD_EXPLICIT_SCHED; // Inherit scheduler = PTHREAD_INHERIT_SCHED

    if (pthread_attr_setinheritsched(&attr1, sched1) != 0)
    {
        printf("Failed attributing inherence attribuits.\n");
        exit(1);
    }
    // ==================================

    return 0;
}
