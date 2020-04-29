#ifndef CIRC_H
#define CIRC_H
// Be warned: these fields have the same name as variables
// This may be problematic (unless you rename) if you use equates in assy.
typedef struct pair_st { int x,y; } pair;
extern int DrawCircle(int r, int i, pair display[]);
#endif

