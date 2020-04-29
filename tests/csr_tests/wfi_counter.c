#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

extern int pwm_hi(int);
extern int pwm_lo(int);

int main() {
  int lo_count = 0;
  int hi_count = 0;
  printf("Read wfi counter\n");
  hi_count = pwm_hi(0);
  lo_count = pwm_lo(0);
  printf("Current read: %x %x \n", hi_count, lo_count);

  sleep(10);
  hi_count = pwm_hi(2020);
  lo_count = pwm_lo(1010);
  printf("WFI count is: %x %x\n", hi_count, lo_count);
  
  return 0;
}
