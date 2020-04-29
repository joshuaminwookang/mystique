//(c) 2020 Josh Kang
// Test to analyze how wfi and idle loops work in lowRISC Rocket processors

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "encoding.h"

// #define SLEEP 1
int main () {
  unsigned long start, end;

#ifdef SLEEP
  start = rdcycle();
  sleep(10);
  end = rdcycle();
  printf("Cycles between sleep for 1 seconds is: %ld\n", end-start);

  #else
  struct timeval init_clock, start_clock, end_clock;

  /* Record start times */
  gettimeofday(&init_clock, NULL);

  /* Delay awhile while holding the CPU */
  do {
    gettimeofday(&start_clock, NULL);
  }
  while (init_clock.tv_sec < start_clock.tv_sec);
  start = rdcycle();
  do {
    gettimeofday(&end_clock, NULL);
  }
  while (start_clock.tv_sec < end_clock.tv_sec);
  end = rdcycle();
  printf("Cycles between %ld system clock useconds is: %ld \n",  1000000 + end_clock.tv_usec - start_clock.tv_usec, end-start);

  gettimeofday(&start_clock, NULL);
  start = rdcycle();
  for (int i=0; i < 72000; i++ ) {gettimeofday(&end_clock, NULL);}
  end = rdcycle();
  printf("Cycles between %ld system clock useconds is: %ld \n", ((end_clock.tv_sec * 1000000 + end_clock.tv_usec)
								 - (start_clock.tv_sec * 1000000 + start_clock.tv_usec)), end-start);
  #endif
  
}
