#define _GNU_SOURCE

#include <stdio.h>
#include "func.h"
#include <time.h>

#define MILLION 1000000.0;

int main( int argc, char **argv )
  {
    struct timespec start, stop;
    double time1, time2, time3;

    // Análise func1
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return -1;;
    }

    f1(0,0);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return -1;
    }

    printf("F1 start sec: %ld  ||| F2 start nsec: %ld\n\n",start.tv_sec,start.tv_nsec);
    printf("F1 stop sec: %ld  ||| F2 stop nsec: %ld\n\n",stop.tv_sec,stop.tv_nsec);

    time1 = ( stop.tv_sec - start.tv_sec )
          + ( stop.tv_nsec - start.tv_nsec )
            / MILLION;


    // Análise func2
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return -1;;
    }

    f2(0,0);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return -1;
    }

    time2 = ( stop.tv_sec - start.tv_sec )
          + ( stop.tv_nsec - start.tv_nsec )
            / MILLION;


    // Análise func3
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return -1;;
    }

    f1(0,0);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return -1;
    }

    time3 = ( stop.tv_sec - start.tv_sec )
          + ( stop.tv_nsec - start.tv_nsec )
            / MILLION;


    printf( " Tempo f1: %lf ms\n Tempo f2: %lf ms\n Tempo f3: %lf ms\n\n", time1,time2,time3);
    return(0);
  }