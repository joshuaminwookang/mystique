
// Library with a dispatch mechanism to support making hw-sw calls for a wide variety of function
// (c) 2020 Josh Kang

#include "rocc.h"
#include "encoding.h"
#include "dispatch.h"
#include "combinations.h"
#include "bloom.h"
#include "strcpy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

// extern inline int  generate_sw (unsigned int inputString, int length, long answer, int funct) 
// extern inline int  generate_hw (unsigned int inputString, int length, long answer, int funct) 

// DNA vector declaration
accMeta DNA[NACC] =
{
    ACCEL(GENERATE0,0, generate0_hw, generate0_sw),
    ACCEL(GENERATE1,0, generate1_hw, generate1_sw),
    ACCEL(GENERATE2,0, generate2_hw, generate2_sw),
    ACCEL(BLOOM_MAP,1, hw_mapWordsFromArray, sw_mapWordsFromArray),
    ACCEL(BLOOM_TEST,1, hw_countMissFromArray, sw_countMissFromArray),
    ACCEL(STRCPY,2, hwStrcpy, strcpy)
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
int generate0 (unsigned int inputString, int length, long answer) {
  if (DNA[GENERATE0].hw_on)
    return ((int (*) ()) DNA[GENERATE0].hw_fun)(inputString, length, answer);
  else
    return ((int (*) ()) DNA[GENERATE0].sw_fun)(inputString, length, answer);
  //  return generate_hw(inputString, length, answer);
}

// Top level "dispatch" functions for each accelerator
int generate1 (unsigned int inputString, int length, long answer) {
  if (DNA[GENERATE1].hw_on)
    return ((int (*) ()) DNA[GENERATE1].hw_fun)(inputString, length, answer);
  else
    return ((int (*) ()) DNA[GENERATE1].sw_fun)(inputString, length, answer);
  //  return generate_hw(inputString, length, answer);
}

// Top level "dispatch" functions for each accelerator
int generate2 (unsigned int inputString, int length, long answer) {
  if (DNA[GENERATE2].hw_on)
    return ((int (*) ()) DNA[GENERATE2].hw_fun)(inputString, length, answer);
  else
    return ((int (*) ()) DNA[GENERATE2].sw_fun)(inputString, length, answer);
  //  return generate_hw(inputString, length, answer);
}
// Top level "dispatch" functions for each accelerator
void mapWordsFromArray (int num) {
  if (DNA[BLOOM_MAP].hw_on)
    return ((void (*) ()) DNA[BLOOM_MAP].hw_fun)(num);
  else
    return ((void (*) ()) DNA[BLOOM_MAP].hw_fun)(num);
  //  return generate_hw(inputString, length, answer);
}
// Top level "dispatch" functions for each accelerator
int countMissFromArray (int num) {
  if (DNA[BLOOM_MAP].hw_on)
    return ((int (*) ()) DNA[BLOOM_TEST].hw_fun)(num);
  else
    return ((int (*) ()) DNA[BLOOM_TEST].hw_fun)(num);
  //  return generate_hw(inputString, length, answer);
}

// Top level "dispatch" functions for each accelerator
char* wstrcpy (const char *dst, const char *src) {
  if (DNA[STRCPY].hw_on)
    return ((char* (*) ()) DNA[STRCPY].hw_fun)(dst,src);
  else
    return ((char* (*) ()) DNA[STRCPY].hw_fun)(dst,src);
  //  return generate_hw(inputString, length, answer);
}

// int wstrcmp (char *a, char *b) {
//      if (heuristic(STRCMP))
//        return DNA[STRCMP].hw_fun(a,b);
//      else
//        return DNA[STRCMP].sw_fun(a,b);
// }

int initDNA()
{
  DNAinitialized = 1;
  DNA[GENERATE0].hw_avail = 1;
  DNA[GENERATE1].hw_avail = 1;
  DNA[GENERATE2].hw_avail = 1;
  DNA[BLOOM_MAP].hw_avail = 1;
  DNA[BLOOM_TEST].hw_avail = 1;
  DNA[STRCPY].hw_avail = 1;


  //check shell environment variable (getenv) for HW/SW use decision set ShellWantsHW
  char* variable;
  if ((variable = getenv("ACCEL"))==NULL) {
    printf("ACCEL not defined. Export whether you want to run HW or not\n");
    exit(1);
  }
  ShellWantsHW = atoi(variable);
  DNA[GENERATE0].hw_on = ShellWantsHW;
  DNA[GENERATE1].hw_on = ShellWantsHW;
  DNA[GENERATE2].hw_on = ShellWantsHW;
  DNA[BLOOM_MAP].hw_on = ShellWantsHW;
  DNA[BLOOM_TEST].hw_on = ShellWantsHW;
  DNA[STRCPY].hw_on = ShellWantsHW;

  // record load statistics
  // set a timer to call initDNA again in a minute
  return 1;
}

int useHWDynamically (int vecNum) {
  if (ShellWantsHW) return 1;
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
