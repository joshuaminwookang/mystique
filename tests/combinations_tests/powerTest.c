// Infinite loop test for power measurements
// (c) Maddie Burbage, 2020 Josh Kang

#include "rocc.h"
#include "encoding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

/* A function to help generate all binary strings of a certain weight.
 * Input the length of the binary string and the previous combination.
 * The pointer, out, will be loaded with the next combination following
 * the suffix-rotation pattern. -1 is returned when the pattern ends.
 * Generation is computed using Knuth's variant on the cool pattern from
 * The Art of Computer Programming, volume 4, fascicle 3.
 */
int nextWeightedCombination(int n, int last, int *out) {
    int next, temp;
    next = last & (last + 1); //Discards trailing ones

    temp = next ^ (next - 1); //Marks the start of the last "10"

    next = temp + 1;
    temp = temp & last;

    next = (next & last) - 1;
    next = (next > 0)? next : 0;

    next = last + temp - next;

    if(next / (1 << n) > 0) {
        return -1;
    }

    *out = next % (1 << n);
    return 1;
}

/* A function to help generate all binary strings of a certain length.
 * The generation is computed using the cool-er pattern from "The Coolest
 * Way to Generate Binary Strings"
 */
int nextGeneralCombination(int n, int last, int *out) {
  unsigned int cut, trimmed, trailed, mask, lastTemp, lastLimit, lastPosition, cap, first, shifted, rotated, result;

    cut = last >> 1;
    trimmed = cut | (cut - 1); //Discards trailing zeros
    trailed = trimmed ^ (trimmed + 1); //Marks the start of the last "01"
    mask = (trailed << 1) + 1;

    lastTemp = trailed + 1; //Indexes the start of the last "01"
    lastLimit = 1 << (n-1); //Indexes the length of the string
    lastPosition = (lastTemp == 0 || lastTemp > lastLimit)? lastLimit : lastTemp;

    cap = 1 << n;
    first = (mask < cap)? 1 & last : 1 & ~(last); //The bit to be moved
    shifted = cut & trailed;
    rotated = (first == 1)? shifted | lastPosition : shifted;
    result = rotated | (~mask & last);

    if(result == cap - 1) {
        return -1;
    }

    *out = result;
    return 1;
}

/* A function to help generate all binary strings of a certain length and weight range
 * The generation is computed using the cool-est pattern from "The Coolest
 * Way to Generate Binary Strings"
 */
int nextRangedCombination(int n, int last, int min, int max, int *out) {
  unsigned int cut, trimmed, trailed, mask, lastTemp, lastLimit, lastPosition, disposable, count, cap, flipped, valid, first, shifted, rotated, result;
    cut = last >> 1;
    trimmed = cut | (cut - 1); //Discards trailing zeros
    trailed = trimmed ^ (trimmed + 1); //Marks the start of the last "01"
    mask = (trailed << 1) + 1;

    lastTemp = trailed + 1; //Indexes the start of the last "01"
    lastLimit = 1 << (n-1); //Indexes the length of the string
    lastPosition = (lastTemp == 0 || lastTemp > lastLimit)? lastLimit : lastTemp;

    disposable = last; //Prepare to count bits set in the string
    for(count = 0; disposable; count++) {
        disposable = disposable & (disposable - 1); //Discard the last bit set
    }

    cap = 1 << n;
    flipped = 1 & ~last;
    valid = (flipped == 0)? count > min : count < max;
    first = (mask < cap || !valid)? 1 & last : flipped; //The bit to be moved
    shifted = cut & trailed;
    rotated = (first == 1)? shifted | lastPosition : shifted;
    result = rotated | (~mask & last);

    cap = (1 << min) - 1;
    if(result == cap) {
        return -1;
    }

    *out = result;
    return 1;
}
static inline int timeHardware(unsigned int inputString, int length, long answer) {
    unsigned int outputString, outputs;

    outputs = 1;

    #if FUNCT % 4 == 0
    length |= (WIDTH/2) << 6;
    #elif FUNCT % 4 == 2
    length |= (0 << 6) |  ((WIDTH/2) << 12);
    #endif
    #if FUNCT < 3
    ROCC_INSTRUCTION_DSS(0, outputString, length, inputString, FUNCT);
    while(outputString != -1) {
	//printf("%d \n", outputString);
	inputString = outputString;
	outputs++;
        ROCC_INSTRUCTION_DSS(0, outputString, length, inputString, FUNCT);
    }
    #else
    unsigned long streamOut[answer];
    ROCC_INSTRUCTION_DSS(0, outputString, length, &streamOut[0], FUNCT);
    outputs = outputString;
    #endif
    return outputs;
}

static inline int timeSoftware(int inputString, int length, int answer) {
    int outputString, outputs;
    outputs = 1;
    #if FUNCT < 3
    while(
	  #if FUNCT % 4 == 0
	  nextWeightedCombination(length, inputString, &outputString)
	  #elif FUNCT % 4 == 1
	  nextGeneralCombination(length, inputString, &outputString)
	  #else
	  nextRangedCombination(length, inputString, 0, WIDTH/2, &outputString)
          #endif
	  != -1) {
	inputString = outputString;
	//printf("%d \n", outputString);
	outputs++;
    }
    #else
    int safe[answer];
    int i = 0;
    while(
	  #if FUNCT % 4 == 0
	  nextWeightedCombination(length, inputString, &safe[i])
	  #elif FUNCT % 4 == 1
	  nextGeneralCombination(length, inputString, &safe[i])
	  #else
	  nextRangedCombination(length, inputString, 0, WIDTH/2, &safe[i])
          #endif
	  != -1) {
	inputString = safe[i];
	//printf("%d \n", inputString);
	i++;
    }
    outputs = (i+1 == answer)? 0 : -1;
    #endif
    return outputs;
}

/* Global loop count variable*/
unsigned long long num_loops = 0;
struct timeval start_tv,end_tv;
long start_time, end_time;

// print number of loops, elapsed time 
void sigintHandler(int dummy)
{
  /* Record End Time */
  gettimeofday(&end_tv,NULL);
  end_time = end_tv.tv_sec%(24*3600);
  
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
  printf ("system uptime : %ld days, %ld:%02ld:%02ld\n",
	  si.uptime / day, (si.uptime % day) / hour,
	  (si.uptime % hour) / minute, si.uptime % minute);
  printf ("total RAM   : %5.1f MB\n", si.totalram / megabyte);
  printf ("free RAM   : %5.1f MB\n", si.freeram / megabyte);
  printf ("process count : %d\n", si.procs);
  printf("load average: %2.2f  %2.2f %2.2f\n", (double) si.loads[0]/load_fshift,(double)  si.loads[1]/load_fshift, (double)  si.loads[2]/load_fshift);

  /* Reset handler to catch SIGINT next time. */
  #if WARE == 1
    printf("\n Results for function: %d with HW \n Elapsed loop count: %lld with 10000 operations per loop\n Elapsed Time: %ld seconds\n",
	 FUNCT, num_loops, end_time-start_time);
  #else
    printf("\n Results for function: %d with SW \n Elapsed loop count: %lld with 1000 operations per loop\n Elapsed Time: %ld seconds\n",
	   FUNCT, num_loops, end_time-start_time);
   #endif
  fflush(stdout);
  exit(0);
} 
int main(void) {
    long startCycle, endCycle;
    //Set input string and the expected number of combinations
    #if FUNCT % 4 == 1 //General combinations
    unsigned long inputString = (1L << WIDTH) - 1;
    long answer = 1L << WIDTH;
    #elif FUNCT % 4 == 0 //Fixed weight combinations
    unsigned long inputString = (1L << WIDTH/2) - 1;
    long lookups[] = {0,0, 2, 0, 6, 0, 20, 0, 70, 0,0,0,0, 1716, 3432,0, 12870,0,0,0, 184756,0,0,0, 2704156,0,0,0,0,0,0,0,601080390};
    long answer = lookups[WIDTH];
    #else //Ranged weight combinations
    unsigned long inputString = 0;
    long lookups[] = {0,0, 3, 0, 11, 0, 42, 0, 163, 0,0,0,0, 4096, 9908,0, 39203,0,0,0, 616666,0,0,0, 9740686,0,0,0,0,0,0,0, 2448023843};
    long answer = lookups[WIDTH];
    #endif
    
    //printf("answer %lu, input %lu \n", answer, inputString);
    //Set the string's length
    int length = WIDTH;
 // Print out number of loops the program underwent when user presses ctrl-c
    signal(SIGINT, sigintHandler);
    
    int testResult = 0;
    gettimeofday(&start_tv,NULL);
    start_time = start_tv.tv_sec%(24*3600);
    while (1) {
      asm volatile ("fence");
      #if WARE == 1
      for (int i = 0; i < 10000; i++) {
	testResult = timeHardware(inputString, length, answer);
      }
      #else
      for (int i = 0; i< 1000; i++) {
	testResult = timeSoftware(inputString, length, answer);
      }
      #endif
      asm volatile ("fence");
      num_loops++;
    }
    
    #if FUNCT < 3
    testResult -= answer;
    #endif
    return testResult;
}

