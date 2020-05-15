/**
 */
#include "rocc.h"


int main(void)
{
  char *a = "Now is the time for all good men to come to the aid of their country.\n";
  char b[100] = "hi\n";
  hwStrcpy(b,a);
  for (int i = 0; i < 100; i++) {
    printf("%02x=%c\n",b[i],((b[i] > 32) && (b[i] < 127))?b[i]:'?');
  }
  return 0;
}
