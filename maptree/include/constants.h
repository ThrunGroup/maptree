/**
 * @file constants.h
 * @brief Constants used throughout the library.
*/

typedef unsigned long long BLOCK;
#define BLOCK_BITS 64
#define FULL_BLOCK 0xFFFFFFFFFFFFFFFFULL
#define NUM_BLOCKS(numBits) ((numBits + BLOCK_BITS - 1) / BLOCK_BITS)