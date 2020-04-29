#include<stdio.h>
#include<stdlib.h>

extern int setwfi(int);

int main(int argc, char** argv) {
  int whatWasThere = setwfi(0);
  printf("Original WFI setting : %d\n", whatWasThere);
  printf("WFI now set to : %d\n", 0);
  return 0;  
}
