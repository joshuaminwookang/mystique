

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>
#include "rocc.h"
#include "encoding.h"
#include "compiler.h"
#include <signal.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include "dispatch.h"

#ifdef __linux
#include <sys/mman.h>
#endif

/* Global loop count variable*/
unsigned long long num_loops = 0;
struct timeval start_tv,end_tv;
long start_time, end_time;

/* Global combinations function variable*/
int funct;

void print_stats(){
  /* Conversion constants. */
  const long minute = 60;
  const long hour = minute * 60;
  const long day = hour * 24;
  const double megabyte = 1024 * 1024;
  const long load_fshift = 1 << 16;
  /* Obtain system statistics. */
  struct sysinfo si;
  sysinfo (&si);
  /* Summarize interesting values. */
  printf ("\nsystem uptime : %ld days, %ld:%02ld:%02ld\n",
	  si.uptime / day, (si.uptime % day) / hour,
	  (si.uptime % hour) / minute, si.uptime % minute);
  printf ("total RAM   : %5.1f MB\n", si.totalram / megabyte);
  printf ("free RAM   : %5.1f MB\n", si.freeram / megabyte);
  printf ("process count : %d\n", si.procs);
  printf("load average: %2.2f  %2.2f %2.2f\n", (double) si.loads[0]/load_fshift,(double)  si.loads[1]/load_fshift, (double)  si.loads[2]/load_fshift);

}
// print number of loops, elapsed time 
void sigintHandler(int dummy)
{
  /* Record End Time */
  gettimeofday(&end_tv,NULL);
  end_time = end_tv.tv_sec%(24*3600);

  print_stats();

  /* Reset handler to catch SIGINT next time. */
  printf("\n Results for function: %d with ACCEL: %d \n Elapsed Loops count: %lld\n Elapsed Time: %ld seconds\n",
	   funct, ShellWantsHW, num_loops, end_time-start_time);
  fflush(stdout);
  //  initDNA();
  //  gettimeofday(&start_tv,NULL);
  //  start_time = start_tv.tv_sec%(24*3600);
  exit(0);
} 

/* FUNCT declarations*/
extern int initDNA();
extern int generate0(unsigned int, int, long);
extern int generate1(unsigned int, int, long);
extern int generate2(unsigned int, int, long);
extern int ShellWantsHW;


/*
 * Test script 
 */
int main(void)
{
    unsigned long start, end;
    int sw_misses = 0;
    

    printf("Beginning Bloom filter SW \n");
    
    // Initialize SW bloom filter array
    memset(bloom_filter_array, 0, M_NUM_BITS);
    // for (int i = 0; i < M_NUM_BITS; i++)
    // {
    //     bloom_filter_array[0] = 0;
    // }

    // SW: Map
    start = rdcycle(); 
    // map words to Bloom filter
    #ifdef TINY
    mapWordsFromArray(TINY);
    #endif
    #ifdef TINYV2
    mapWordsFromArray(TINYV2);
    #endif
    #ifdef TINYV3_MAP
    mapWordsFromArray(TINYV3_MAP);
    #endif
    #ifdef TINYV3_TEST
    mapWordsFromArray(30);
    #endif
    end = rdcycle();  
    printf("SW MAP execution took %lu cycles\n", end - start); 

    // SW: TEST
    start = rdcycle(); 
    // test if words in file 2 are in Bloom filter
    #ifdef TINY
    sw_misses = countMissFromArray(TINY);
    #endif
    #ifdef TINYV2
    sw_misses = countMissFromArray(TINYV2);
    #endif
    #ifdef TINYV3_MAP
    sw_misses = countMissFromArray(30);
    #endif
    #ifdef TINYV3_TEST
    sw_misses = countMissFromArray(TINYV3_TEST);
    #endif
    end = rdcycle(); 

    // print out info
    printf("SW TEST execution took %lu cycles\n", end - start); 
    printf("Software Misses: %d\n", sw_misses);

    return 0;
}
