#include <signal.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include "rocc.h"
#include "encoding.h"
#include "dispatch.h"

/* Global loop count variable*/
unsigned long long num_loops = 0;
struct timeval start_tv,end_tv;
long start_time, end_time;

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
  printf("\n STRCPY Results with ACCEL: %d \n Elapsed Loops count: %lld\n Elapsed Time: %ld seconds\n",
	    ShellWantsHW, num_loops, end_time-start_time);  
  fflush(stdout);
  //  initDNA();
  //  gettimeofday(&start_tv,NULL);
  //  start_time = start_tv.tv_sec%(24*3600);
  exit(0);
} 

/* FUNCT declarations*/
extern int initDNA();
extern char* wstrcpy(const char *, const char *);
extern int ShellWantsHW;

/*
 * Test script 
 */
int main(int argc, char **argv) {
  
  // Read in new ACCEL environemnt variable and reset HW or SW
  signal(SIGINT, sigintHandler);

  gettimeofday(&start_tv,NULL);
  start_time = start_tv.tv_sec%(24*3600);

  char *a = "Now is the time for all good men to come to the aid of their country.\n";
  char b[100] = "hi\n";
  while (1) {
    for (int i = 0; i < 100; i++) {
      wstrcpy(b,a);
      num_loops++;
    }
  }
  return 0;
}
