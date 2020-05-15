#include "rocc.h"
#include "encoding.h"

static inline char* hwStrcpy(const char *dst, const char *src)
{
  unsigned long int s = (unsigned long int)src;
  unsigned long int d = (unsigned long int)dst;
  asm volatile ("fence");
  ROCC_INSTRUCTION_SS(2, s, d, 0);
  asm volatile ("fence");
  return (char*) d;
}