/***
block.h

Interface for 64 bit block used by PRINCE cipher

by: Jennifer Tang
***/

#ifndef _BLOCK_INCLUDED_
#define _BLOCK_INCLUDED_

#include <inttypes.h>

/*
A block is a sequence of 64 bits. It will be saved as two integers.
The integer MS (most signficant) will represent the first 32 bits, and
the integer LS (least significant) will represent to last 32 bits
 */
typedef struct block {
    uint32_t MS;
    uint32_t LS;
} block_t;

enum{NIBBLE_SIZE = 4};
enum{NUM_OF_NIBBLES = 16};

/* creates and returns new block with MS as most significant 32 digits
 and LS as least significant 32 digits*/
block_t block_new(unsigned int MS, unsigned int LS);

/* creates new block from random values */
block_t block_newRandom();

/* creates new block from string */
block_t block_newFromString(char blockString[NUM_OF_NIBBLES + 1]);

/* sets the value of block so that MS is first 32 bits
and LS is the last 32 bits */
void block_set(block_t* block, int MS, int LS);

/* state becomes state xor value */
void block_xor(block_t* state, block_t value);

/* returns an integer corresponding to the 4-bit nibble at index in block
   nibble 0 is the most significant 4 bits
*/
int block_getNibble(block_t block, int index);

/* set the 4-bit nibble at index in block. Returns 0 if no error
   nibble 0 is the most signifcant 4 bits
*/
int block_setNibble(block_t* block, int index, int nibble);

/* sets block to a random block */
void block_getRandom(block_t* block);

/* prints block */
void block_print(block_t block);

/* sets str to block expressed in hex*/
void block_toString(block_t block, char str[NUM_OF_NIBBLES + 1]);

#endif

