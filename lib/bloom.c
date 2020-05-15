

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>
#include "rocc.h"
#include "encoding.h"
#include <signal.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include "dispatch.h"

#define BUF_SIZE 100     // max size of word
#define M_NUM_BITS 1000 // number of elements in Bloom filter
#define K_NUM_HASH 5     // number of hash functions
#define HASH_NUM 5381    // number used for hash function
#include "small_data.h"

/* Global loop count variable*/
unsigned long long num_loops = 0;
struct timeval start_tv,end_tv;
long start_time, end_time;

int funct;
int misses;

/* global Bloom bit array */
extern unsigned char bloom_filter_array[M_NUM_BITS];

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
  printf("\n Bloom Results for function: %d with ACCEL: %d \n Elapsed Loops count: %lld\n Elapsed Time: %ld seconds\n",
	   funct, ShellWantsHW, num_loops, end_time-start_time);
  printf("Misses : %d\n", misses);
  
  fflush(stdout);
  //  initDNA();
  //  gettimeofday(&start_tv,NULL);
  //  start_time = start_tv.tv_sec%(24*3600);
  exit(0);
} 

/* FUNCT declarations*/
extern int initDNA();
extern void mapWordsFromArray(int);
extern int countMissFromArray(int);
extern int ShellWantsHW;


/*
 * Test script 
 */
int main(int argc, char **argv) {
  funct = 0; misses=0;
  if (argc > 1) funct = atoi(argv[1]); 
  if(initDNA() < 0) exit(1);

  printf("Beginning Bloom filter for function: %d\n", funct);
  
  // Initialize SW bloom filter array
  memset(bloom_filter_array, 0, M_NUM_BITS);
  // Read in new ACCEL environemnt variable and reset HW or SW
  signal(SIGINT, sigintHandler);

  gettimeofday(&start_tv,NULL);
  start_time = start_tv.tv_sec%(24*3600);


  switch (funct){
    case 0:
      while (1) {
        asm volatile ("fence");
        for (int i = 0; i < 100; i++) {
	        mapWordsFromArray(50);
		      num_loops++;
        }
        asm volatile ("fence");
      } 
      break;
    case 1:
      while (1) {
        asm volatile ("fence");
        for (int i = 0; i < 100; i++) {
	        misses = countMissFromArray(50);
		      num_loops++;
        }
        asm volatile ("fence");
      } 
      break;
    default:
      break;
  }

    return 0;
}
