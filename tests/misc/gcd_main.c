#include<stdio.h>
#include<stdlib.h>

extern int gcd(int, int);

int main(int argc,char **argv)
{
  if(argc!=3){
    printf("Input: int int\n");
    exit(1);
  }  
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  printf("GCD of %d and %d is %d\n",a,b,gcd(a,b));
  return 0;
}
