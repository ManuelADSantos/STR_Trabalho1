#define _GNU_SOURCE

#include <stdio.h>
#include "func.h"
#include <time.h>

#include "timestamps.h"

#define MILLION 1000000.0
#define TEST_ITERATIONS 20

int main(int argc, char **argv)
{
  struct timespec start, stop;
  double time1 = 1000, time2 = 1000, time3 = 1000, calc = 0;

  printf("\n");
  // ========== Análise func1 ==========
  for (int i = 0; i < TEST_ITERATIONS; i++)
  {
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

    // printf("    F1 start sec: %ld  ||| F1 start nsec: %ld\n", start.tv_sec, start.tv_nsec);
    // printf("    F1 stop sec: %ld  ||| F1 stop nsec: %ld\n", stop.tv_sec, stop.tv_nsec);

    calc = time_between_timestamp(start, stop);
    // printf("  (func1) = %lf ms (iteracao %d)\n", calc, i);
    if (calc < time1)
    {
      printf("Novo melhor (func1) = %lf ms (iteracao %d)\n", calc, i);
      time1 = calc;
    }
  }

  // printf(" ---> O melhor tempo foi %lf ms (func1)\n\n\n", time1);

  printf("\n");
  //  ========== Análise func2 ==========
  for (int i = 0; i < TEST_ITERATIONS; i++)
  {
    if (clock_gettime(CLOCK_REALTIME, &start) == -1)
    {
      perror("clock gettime");
      return -1;
    }

    f2(0, 0);

    if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
    {
      perror("clock gettime");
      return -1;
    }

    // printf("F1 start sec: %ld  ||| F1 start nsec: %ld\n\n", start.tv_sec, start.tv_nsec);
    // printf("F1 stop sec: %ld  ||| F1 stop nsec: %ld\n\n", stop.tv_sec, stop.tv_nsec);

    calc = time_between_timestamp(start, stop);
    // printf("  (func2) = %lf ms (iteracao %d)\n", calc, i);
    if (calc < time2)
    {
      printf("Novo melhor (func2) = %lf ms (iteracao %d)\n", calc, i);
      time2 = calc;
    }
  }

  // printf(" ---> O melhor tempo foi %lf ms (func2)\n\n\n", time2);
  printf("\n");
  //  ========== Análise func3 ==========
  for (int i = 0; i < TEST_ITERATIONS; i++)
  {
    if (clock_gettime(CLOCK_REALTIME, &start) == -1)
    {
      perror("clock gettime");
      return -1;
    }

    f3(0, 0);

    if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
    {
      perror("clock gettime");
      return -1;
    }

    // printf("F1 start sec: %ld  ||| F1 start nsec: %ld\n\n", start.tv_sec, start.tv_nsec);
    // printf("F1 stop sec: %ld  ||| F1 stop nsec: %ld\n\n", stop.tv_sec, stop.tv_nsec);

    calc = time_between_timestamp(start, stop);
    // printf("  (func3) = %lf ms (iteracao %d)\n", calc, i);
    if (calc < time3)
    {
      printf("Novo melhor (func3) = %lf ms (iteracao %d)\n", calc, i);
      time3 = calc;
    }
  }

  // printf(" ---> O melhor tempo foi %lf ms (func3)\n\n\n", time3);
  printf("\n");

  printf(" Tempo f1: %lf ms\n Tempo f2: %lf ms\n Tempo f3: %lf ms\n\n", time1, time2, time3);

  return (0);
}