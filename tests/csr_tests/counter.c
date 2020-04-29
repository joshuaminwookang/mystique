#include<stdio.h>
#include<stdlib.h>

extern int CsrRW(int);

int main(int argc, char** argv) {
  int input = atoi(argv[1]);
  for (int i=1; i<=(input+1);i++) {
    printf("CSR register value: %d\n",CsrRW(i));
  }
  return 0;  
}
