// A program to drive the assembly version of DrawCircle.
#include <stdio.h>
#include <stdlib.h>
#include "circ.h"

int main(int argc, char **argv)
{
  pair display[100];
  int i;
  int r = (argc>1)?atoi(argv[1]):10;
  int n = DrawCircle(r,0,display);
  for (i = 0; i < n; i++) {
    printf("(%d,%d)\n",display[i].x,display[i].y);
  }
  return 0;
}
