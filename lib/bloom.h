
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

#define BUF_SIZE 100     // max size of word
#define M_NUM_BITS 1000 // number of elements in Bloom filter
#define K_NUM_HASH 5     // number of hash functions
#define HASH_NUM 5381    // number used for hash function
#include "small_data.h"

#define TINYV3_MAP  50

/* global Bloom bit array */
unsigned char bloom_filter_array[M_NUM_BITS];

/*
 * Hash function for a string using Horner's Rule.
 * Given a string, returns a number.
 */
unsigned long hashstring(char* word)
{
    unsigned char *str = (unsigned char *)word;
    unsigned long hash = HASH_NUM;

    // while there are still chars in the word
    while (*str)
    {
        // hash = (hash * 32) + hash + current char in word
        hash = ((hash << 5) + hash) + *(str++);
    }

    return hash;
}

/*
 * map word to bloom filter.
 * Places 1 in filter at indices that given word maps to.
 */
void mapToBloom(int index)
{
    #ifdef TINY
    long x = hashstring(tiny0[index]); 
    #endif
    #ifdef TINYV2
    long x = hashstring(tiny2[index]); 
    #endif
    #ifdef TINYV3_MAP
    long x = hashstring(tiny4[index]); 
    #endif
    #ifdef TINYV3_TEST
    long x = hashstring(tiny2[index]); 
    #endif
    
    long y = x >> 4;

    for (int i = 0; i < K_NUM_HASH; i++)
    {
        x = (x + y) % M_NUM_BITS; // ith hash value
        y = (y + i) % M_NUM_BITS; // displacement
        bloom_filter_array[x] = 1;
    }
}

/*
 * Reads words from array and maps them to Bloom filter.
 */
void mapWordsFromArray(int num)
{
    for (int i = 0; i < num; i++)
    {
        mapToBloom(i);
    }
}

/*
 * tests if word is in bloom filter.
 * Tests if there is a 1 in filter at indices 
 * that given word maps to.
 *
 * Returns 1 if search is positive, 0 if negative.
 */
int testBloom(int index)
{
    #ifdef TINY
    long x = hashstring(tiny1[index]); 
    #endif
    #ifdef TINYV2
    long x = hashstring(tiny3[index]); 
    #endif
    #ifdef TINYV3_MAP
    long x = hashstring(tiny3[index]); 
    #endif
    #ifdef TINYV3_TEST
    long x = hashstring(tiny4[index]); 
    #endif
    long y = x >> 4; 

    for (int i = 0; i < K_NUM_HASH; i++)
    {
        x = (x + y) % M_NUM_BITS; // ith hash value
        y = (y + i) % M_NUM_BITS; // displacement

        if (!bloom_filter_array[x])
        {
            return 0;
        }
    }

    return 1;
}

int countMissFromArray(int num)
{
    int count = 0;

    for (int i = 0; i < num; i++)
    {
        if (!testBloom(i))
        {
            count++;
        }
    }

    return count;
}


