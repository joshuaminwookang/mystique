#include<stdio.h>

extern void wfi();

int main(void) {
  while (1){ wfi();}
    return 0;
}
