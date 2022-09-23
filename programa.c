#define _GNU_SOURCE

#include <stdio.h>
#include "func.h"
#include <time.h>

#define MILLION 1000000.0;

int main(int argc, char **argv)
{
  struct timespec start, stop;
  double time1 = 1000, time2 = 1000, time3 = 1000, calc = 0;

  for (int i = 0; i < 10; i++)
  {
    // Análise func1
    if (clock_gettime(CLOCK_REALTIME, &start) == -1)
    {
      perror("clock gettime");
      return -1;
    }

    f1(0, 0);

    if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
    {
      perror("clock gettime");
      return -1;
    }

    // printf("F1 start sec: %ld  ||| F2 start nsec: %ld\n\n", start.tv_sec, start.tv_nsec);
    // printf("F1 stop sec: %ld  ||| F2 stop nsec: %ld\n\n", stop.tv_sec, stop.tv_nsec);

    calc = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / MILLION;
    if (calc < time1)
    {
      printf("Novo melhor = %lf ms (iteracao %d)\n", calc, i);
      time1 = calc;
    }
  }

  // Análise func2
  if (clock_gettime(CLOCK_REALTIME, &start) == -1)
  {
    perror("clock gettime");
    return -1;
    ;
  }

  f2(0, 0);

  if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
  {
    perror("clock gettime");
    return -1;
  }

  time2 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / MILLION;

  // Análise func3
  if (clock_gettime(CLOCK_REALTIME, &start) == -1)
  {
    perror("clock gettime");
    return -1;
    ;
  }

  f3(0, 0);

  if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
  {
    perror("clock gettime");
    return -1;
  }

  time3 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / MILLION;

  printf(" Tempo f1: %lf ms\n Tempo f2: %lf ms\n Tempo f3: %lf ms\n\n", time1, time2, time3);
  return (0);
}