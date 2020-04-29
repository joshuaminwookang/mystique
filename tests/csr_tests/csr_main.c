#include<stdio.h>
#include<stdlib.h>

extern int CsrRW(int);

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: csr [value to write to CSR register]\n");
    exit(1);
  }
  int input = atoi(argv[1]);
  int whatWasThere = CsrRW(input);
  printf("Original value: %d\n", whatWasThere);
  printf("Changed value to : %d\n", input);
  return 0;  
}
