#ifndef DISPATCH_H
#define DISPATCH_H
// Metadata for each HW accelerator (possible)
typedef struct accMeta {
  int index;
  int customId;
  void (*hw_fun)();
  void (*sw_fun)();
  int hw_avail; // 1 if accelerator is built-in (available)
  int hw_on; // 1 if accelerator is turned-on (set to be used)
  int calls ; // # of calls
  int speed_reward;
  int power_reward;
  // other data and metrics to make judgements about hw vs sw
  //int loadfactor_on; int loadfactor_off;
} accMeta;

// reference to actual DNA vector declared in dispatch.c
#define ACCEL(name,cid,hwname,swname) { name,cid, (void (*) ()) hwname,(void (*) ()) swname,0,0,0,0 }
extern accMeta DNA[];

// Initialize DNA vector; parameters and function calls
// Also records current load average / call rates
extern int DNAinitialized;
extern int ShellWantsHW;
extern int initDNA();

// list of accelerator dispatch functions
#define GENERATE0 0
extern int generate0(unsigned int, int, long);

#define GENERATE1 1
extern int generate1(unsigned int, int, long);

#define GENERATE2 2
extern int generate2(unsigned int, int, long);

// #define BLOOM 1
// extern int bloom(unsigned int, int, long);
// //extern int generate(unsigned int, int, long, int);

// #define STRCPY   2
// extern int wstrcpy(char *, char*);
// #define STRCMP   3
// extern int wstrcmp(char *, char*);

#define NACC 1 // number of accelerators

// different declarations of the heuristic function
typedef int (*heurFun_t)(int);

extern heurFun_t heuristic;
// extern heurFun_t useHWDynamically;
// extern heurFun_t useSW;
// extern heurFun_t useHW;
// extern heurFun_t shellMediatedHW;


#define WIDTH 16
#endif
