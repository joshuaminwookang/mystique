// Tests for the general combination accelerator that stores to memory
// (c) Maddie Burbage, 2020


#include "rocc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A function to help generate all binary strings of a certain length.
 * The generation is computed using the cool-er pattern from "The Coolest
 * Way to Generate Binary Strings"
 */
int nextGeneralCombination(int n, int last, int *out) {
  unsigned int cut, trimmed, trailed, mask, lastTemp, lastLimit, lastPosition, cap, first, shifted, rotated, result;

    cut = last >> 1;
    trimmed = cut | (cut - 1); //Discards trailing zeros
    trailed = trimmed ^ (trimmed + 1); //Marks the start of the last "01"
    mask = (trailed << 1) + 1;
    
    lastTemp = trailed + 1; //Indexes the start of the last "01"
    lastLimit = 1 << (n-1); //Indexes the length of the string
    lastPosition = (lastTemp == 0 || lastTemp > lastLimit)? lastLimit : lastTemp;

    cap = 1 << n;
    first = (mask < cap)? 1 & last : 1 & ~(last); //The bit to be moved
    shifted = cut & trailed;
    rotated = (first == 1)? shifted | lastPosition : shifted;
    result = rotated | (~mask & last);

    if(result == cap - 1) {
        return -1;
    }

    *out = result;
    return 1;
}


static inline int testAccelerator(int length) {
    int sum;

    long int placeholder[64]; //long 64
    int i = 0;
    for(i; i < 64; i++) {
	placeholder[i] = i;
    }
    
    length = length | (0 << 5) | (6 << 10);
    ROCC_INSTRUCTION_DSS(0, sum, length, &placeholder[0], 5);

    for(i = 0; i < 64; i++) {
	printf("spot %d reads %x address %x\n", i, placeholder[i], &placeholder[i]);
    }
    return sum;
}

int main(void) {
    int length = 4;
    int testResult = testAccelerator(length);
    return testResult;
}
