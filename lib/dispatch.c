
// Library with a dispatch mechanism to support making hw-sw calls for a wide variety of function
// (c) 2020 Josh Kang

#include "rocc.h"
#include "encoding.h"
#include "dispatch.h"
//#include "combinations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

#define LONGTOP 0x8000000000000000
#define WIDTH 16
#define FUNCT 2

/* A function to help generate all binary strings of a certain weight.
 * Input the length of the binary string and the previous combination.
 * The pointer, out, will be loaded with the next combination following
 * the suffix-rotation pattern. -1 is returned when the pattern ends.
 * Generation is computed using Knuth's variant on the cool pattern from
 * The Art of Computer Programming, volume 4, fascicle 3.
 */ 
int nextWeightedCombination(long n, unsigned long last, unsigned int *out) {
    unsigned long next, temp, result;
    next = last & (last + 1); //Discards trailing ones
    temp = next ^ (next - 1); //Marks the start of the last "10"

    next = temp + 1;
    temp = temp & last;

    next = (next & last) - 1;

    next = (next < LONGTOP)? next : 0;

    result = last + temp - next;

    if(result / (1L << n) > 0) {
        return -1;
    }

    *out = result % (1L << n);
    return 1;
}

/* A function to help generate all binary strings of a certain length.
 * The generation is computed using the cool-er pattern from "The Coolest
 * Way to Generate Binary Strings"
 */
int nextGeneralCombination(long n, unsigned long last, unsigned int *out) {
  unsigned long cut, trimmed, trailed, mask, lastTemp, lastLimit, lastPosition, cap, first, shifted, rotated, result;

    cut = last >> 1;
    trimmed = cut | (cut - 1); //Discards trailing zeros
    trailed = trimmed ^ (trimmed + 1); //Marks the start of the last "01"
    mask = (trailed << 1) + 1;

    lastTemp = trailed + 1; //Indexes the start of the last "01"
    lastLimit = 1L << (n-1); //Indexes the length of the string
    lastPosition = (lastTemp == 0 || lastTemp > lastLimit)? lastLimit : lastTemp;

    cap = 1L << n;
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
int nextRangedCombination(long n, unsigned long last, long min, long max, unsigned int *out) {
  unsigned long cut, trimmed, trailed, mask, lastTemp, lastLimit, lastPosition, disposable, count, cap, flipped, valid, first, shifted, rotated, result;
    cut = last >> 1;
    trimmed = cut | (cut - 1); //Discards trailing zeros
    trailed = trimmed ^ (trimmed + 1); //Marks the start of the last "01"
    mask = (trailed << 1) + 1;

    lastTemp = trailed + 1; //Indexes the start of the last "01"
    lastLimit = 1L << (n-1); //Indexes the length of the string
    lastPosition = (lastTemp == 0 || lastTemp > lastLimit)? lastLimit : lastTemp;

    disposable = last; //Prepare to count bits set in the string
    for(count = 0; disposable; count++) {
        disposable = disposable & (disposable - 1); //Discard the last bit set
    }

    cap = 1L << n;
    flipped = 1 & ~last;
    valid = (flipped == 0)? count > min : count < max;
    first = (mask < cap || !valid)? 1 & last : flipped; //The bit to be moved
    shifted = cut & trailed;
    rotated = (first == 1)? shifted | lastPosition : shifted;
    result = rotated | (~mask & last);

    cap = (1L << min) - 1;
    if(result == cap) {
        return -1;
    }

    *out = result;
    return 1;
}

// generate sw
static inline int generate_sw(unsigned int inputString, int length, long answer) {
    unsigned int outputString, outputs;
    outputs = 1;
    // switch(funct) {
    //     case 0:
    //         while( nextWeightedCombination(length, inputString, &outputString) != -1) {
	  //           inputString = outputString;
	  //           outputs++;
    //         }
    //         break;
    //     case 1:
    //         while( nextGeneralCombination(length, inputString, &outputString) != -1) {
	  //           inputString = outputString;
	  //           outputs++;
    //         }
    //         break;
    //     case 2:
    //         while( nextRangedCombination(length, inputString, 0, WIDTH/2, &outputString) != -1) {
	  //           inputString = outputString;
	  //           outputs++;
    //         }
    //         break;
    //     default:
    //         break;
    // }
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
    return outputs;
}

static inline int  generate_hw (unsigned int inputString, int length, long answer) {
    unsigned int outputString, outputs;
    outputs = 1;

    #if FUNCT % 4 == 0
    length |= (WIDTH/2) << 6;
    #elif FUNCT % 4 == 2
    length |= (0 << 6) |  ((WIDTH/2) << 12);
    #endif

    ROCC_INSTRUCTION_DSS(0, outputString, length, inputString, FUNCT);
    while(outputString != -1) {
	    //printf("%d \n", outputString);
	    inputString = outputString;
	    outputs++;
      ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, FUNCT);
    } 
    // switch(funct) {
    //     case 0: 
    //         length |= (WIDTH/2) << 6;
    //         ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, 0);
    //         while(outputString != -1) {
	  //           //printf("%d \n", outputString);
	  //           inputString = outputString;
	  //           outputs++;
    //              ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, 0);
    //         }
    //         break;
    //     case 1:
    //         ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, 1);
    //         while(outputString != -1) {
	  //           //printf("%d \n", outputString);
	  //           inputString = outputString;
	  //           outputs++;
    //              ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, 1);
    //         }      
    //         break;     
    //     case 2:
    //         length |= (0 << 6) |  ((WIDTH/2) << 12);
    //         ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, 2);
    //         while(outputString != -1) {
	  //           //printf("%d \n", outputString);
	  //           inputString = outputString;
	  //           outputs++;
    //              ROCC_INSTRUCTION_DSS(1, outputString, length, inputString, 2);
    //         }
    //         break;
    //     default:
    //         break;
    // }
    return outputs;
}

// DNA vector declaration
accMeta DNA[NACC] =
{
    ACCEL(GENERATE,0, generate_hw, generate_sw)
    //ACCEL(STRCMP,1,strcmp_hw,strcmp_sw),    
};

// currently selected heurstic function
extern int useHWDynamically(int);
extern int useHW(int);
extern int useSW(int);
extern int shellMediatedHW(int);
heurFun_t heuristic = &useHWDynamically;

// static timer global_timer;

// Top level "dispatch" functions for each accelerator
int generate (unsigned int inputString, int length, long answer) {
  if (DNA[GENERATE].hw_on)
    return ((int (*) ()) DNA[GENERATE].hw_fun)(inputString, length, answer);
  else
    return ((int (*) ()) DNA[GENERATE].sw_fun)(inputString, length, answer);
  //  return generate_hw(inputString, length, answer);
}

// // Top level "dispatch" functions for each accelerator
// int generate (unsigned int inputString, int length, long answer, int funct) {
//   if (DNA[GENERATE].hw_on)
//     return ((int (*) ()) DNA[GENERATE].hw_fun)(inputString, length, answer, funct);
//   else
//     return ((int (*) ()) DNA[GENERATE].sw_fun)(inputString, length, answer, funct);
//   //  return generate_hw(inputString, length, answer);
// }

// int wstrcmp (char *a, char *b) {
//      if (heuristic(STRCMP))
//        return DNA[STRCMP].hw_fun(a,b);
//      else
//        return DNA[STRCMP].sw_fun(a,b);
// }

int initDNA()
{
  DNAinitialized = 1;
  DNA[GENERATE].hw_avail = 1;
  //check shell environment variable (getenv) for HW/SW use decision set ShellWantsHW
  char* variable;
  if ((variable = getenv("ACCEL"))==NULL) {
    printf("ACCEL not defined. Export whether you want to run HW or not\n");
    exit(1);
  }
  ShellWantsHW = atoi(variable);
  DNA[GENERATE].hw_on = ShellWantsHW;
  // record load statistics
  // set a timer to call initDNA again in a minute
  return 1;
}

int useHWDynamically (int vecNum) {
  //if (!DNAinitialized) return initDNA();
  if (ShellWantsHW) return 1;
    
  /* if(DNA[vecNum].hw_avail) { */
  /*   return 1; */
  /* } */
    // Heuristic decision function
  return 0; 
}

int useSW(int vecNum){
  return 0;
}

int useHW(int vecNum){
  return DNA[vecNum].hw_avail;
}

// int shellMediatedHW(int vecNum){ check ShellWantsHW}
int DNAinitialized = 0; // if not initialized
int ShellWantsHW = 0;
