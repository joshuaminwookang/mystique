

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>
#include "rocc.h"
#include "encoding.h"
#include "compiler.h"

#ifdef __linux
#include <sys/mman.h>
#endif

#define BUF_SIZE 100     // max size of word
#define M_NUM_BITS 20000 // number of elements in Bloom filter
#define K_NUM_HASH 5     // number of hash functions
#define HASH_NUM 5381    // number used for hash function
// #define TINY 11
//#define TINYV2 30
 #define TINYV3_MAP  50
// #define TINYV3_TEST  50
// #define SMALL 10000

#include "small_data.h"





/*
 * Test script 
 */
int main(void)
{
    unsigned long start, end;
    int hw_misses = 0;

    printf(" Beginning HW test for MAP()\n");
    // Initalize BF Accelerator
    // asm volatile ("fence");
    // hw_initBloom();

    // HW: MAP
    start = rdcycle();                                                                                                                                      
    // asm volatile ("fence");
    #ifdef TINY
        hw_mapWordsFromArray(TINY);
    #endif 
    #ifdef TINYV2
        hw_mapWordsFromArray(TINYV2);
    #endif 
    #ifdef TINYV3_MAP
        hw_mapWordsFromArray(TINYV3_MAP);
    #endif 
    #ifdef TINYV3_TEST
        hw_mapWordsFromArray(30);
    #endif 
    #ifdef SMALL       
        returnValue = hw_mapToBloom(hashstring(small[i]));
    #endif
    // asm volatile ("fence");
    end = rdcycle();
    printf("MAP execution took %lu cycles\n", end - start);

    // HW: TEST
    start = rdcycle();  
    // asm volatile ("fence");
    #ifdef TINY
        hw_misses = hw_countMissFromArray(TINY);
    #endif 
    #ifdef TINYV2
        hw_misses = hw_countMissFromArray(TINYV2);
    #endif 
    #ifdef TINYV3_MAP
        hw_misses = hw_countMissFromArray(30);
    #endif 
    #ifdef TINYV3_TEST
        hw_misses = hw_countMissFromArray(TINYV3_TEST);
    #endif 
    #ifdef SMALL       
        returnValue = hw_mapToBloom(hashstring(tiny2[i]));
    #endif
    
    // asm volatile ("fence");
    end = rdcycle();   
    printf("TEST execution took %lu cycles\n", end - start);
    // print out test results
    printf("------\nTotal Hardware Miss: %d \n------\n", hw_misses);

    return 0;
}
