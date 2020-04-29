#include<stdio.h>

extern int debug(int);

// Test if 
int main(void) {
  printf("check if usingRoCC is set:\n");
  printf("test: %d\n", 1 == 0);
  printf("%d\n", debug(0));
  return 0;
}
