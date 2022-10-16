#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>

#include "func.h"
#include "timestamps.h"

#define TEST_ITERATIONS 20

int main(int argc, char **argv)
{
  struct timespec start, stop;
  double calc = 0;
  double times[3][2] = {{__DBL_MAX__, 0}, {__DBL_MAX__, 0}, {__DBL_MAX__, 0}};
  cpu_set_t mask;

  if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
    return -1;

  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  if (sched_setaffinity(getpid(), sizeof(mask), &mask) == -1)
    return -1;

  printf("\n");

  for (int j = 0; j < 3; j++)
  {

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
      if (clock_gettime(CLOCK_REALTIME, &start) == -1)
        return -1;

      if (j == 0)
        f1(0, 0);
      if (j == 1)
        f2(0, 0);
      if (j == 2)
        f3(0, 0);

      if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
        return -1;

      calc = time_between_timestamp(start, stop);
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

  printf(" === Tempos Minimos ===\n    f1: %lf ms\n    f2: %lf ms\n    f3: %lf ms\n\n",
         times[0][0], times[1][0], times[2][0]);
  printf(" === Tempos Maximos ===\n    f1: %lf ms\n    f2: %lf ms\n    f3: %lf ms\n\n",
         times[0][1], times[1][1], times[2][1]);

  return (0);
}