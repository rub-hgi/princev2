/**
block.c

Implementation for 64 bit block used by PRINCEv2 cipher

The 64 bit sequence is broken up into 16 nibbles, numbered from 0 to 15.
Nibble 0 is first 4 bits in the sequence and nibble 15 is the last 4 bits
in the sequence.
**/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"

/* creates and returns new block with MS as most significant 32 digits
 and LS as least significant 32 digits. MS and LS need  to be unsigned
 for bit shift operations to work correctly.
*/
block_t block_new(unsigned int MS, unsigned int LS) {
    block_t oBlock = {.MS = MS, .LS = LS};
    return oBlock;
}

/* creates new block from random values */
block_t block_newRandom() {
    block_t oBlock = {.MS = rand(), .LS = rand()};

    return oBlock;
}

/* get int from char */
static int getInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >='a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    fprintf(stderr, "getInt: invalid character! must be 0-9 or a-f\n");
    exit(-1);
}

/* creates new block from string */
block_t block_newFromString(char blockString[NUM_OF_NIBBLES + 1]) {
    unsigned int value[2];

    int valueIndex;
    int placeIndex;
    int stringIndex = 0;

    assert(strlen(blockString) == NUM_OF_NIBBLES);

    for (valueIndex = 0; valueIndex < 2; valueIndex++) {
        value[valueIndex] = 0;
        for (placeIndex = 7; placeIndex >= 0; placeIndex--) {
            int num = getInt(blockString[stringIndex]);
            value[valueIndex] +=  (((unsigned int)num) << 4 * placeIndex);

            stringIndex++;
        }
    }

    return block_new(value[0], value[1]);
}


/* copies src to dest */
void block_copy(block_t src, block_t* dest) {
    assert(dest != NULL);

    dest->MS = src.MS;
    dest->LS = src.LS;
}

/* sets the value of block so that MS is first 32 bits
and ls is the last 32 bits */
void block_set(block_t* block, int MS, int LS) {
    assert(block != NULL);

    block->MS = MS;
    block->LS = LS;
}

/* state becomes state xor value*/
void block_xor(block_t* state, block_t value) {
    assert(state != NULL);

    state->MS = state->MS ^ value.MS;
    state->LS = state->LS ^ value.LS;
}

/* returns an integer corresponding to the 4-bit nibble at index in block */
int block_getNibble(block_t block, int index) {
    assert(index >= 0 && index < NUM_OF_NIBBLES);

    if (index < NUM_OF_NIBBLES / 2) {
        return ((block.MS) >> (NIBBLE_SIZE * ( 7 - index))) & 0xf;
    } else {
        return ((block.LS) >> (NIBBLE_SIZE * (15 - index))) & 0xf;
    }
}

/* set the 4-bit nibble at index in block.
   returns 0 if no error */
int block_setNibble(block_t* block, int index, int nibble) {
    assert(block != NULL);

    /* only the first 4 bits can be nonzero */
    if (index >= NUM_OF_NIBBLES || index < 0) {
        fprintf(stderr, "block_setNibble: index too large \n");
        return -1; /* error */
    }

    /* nibble is invalid */
    if (nibble > 15) {
        fprintf(stderr, "block_setNibble: nibble value too large \n");
        return -1; /* error */
    }
    if (index < NUM_OF_NIBBLES / 2) {
        int position = (NIBBLE_SIZE *(7 - index));
        block->MS -= (block->MS & (15  << position));
        block->MS = ((nibble) << position) ^ block->MS;
    } else {
        int position = (NIBBLE_SIZE *(15 - index));
        block->LS -= (block->LS & (15  << position));
        block->LS = ((nibble) << position) ^ block->LS;
    }

    return 0;
}

/* sets block to a random value */
void block_getRandom(block_t* block) {
    assert(block != NULL);

    block->MS = rand();
    block->LS = rand();
}

/* prints block */
void block_print(block_t block) {
    printf("%08x%08x\n", block.MS, block.LS);
}

/* sets str to block expressed in hex*/
void block_toString(block_t block, char str[NUM_OF_NIBBLES + 1]) {
    sprintf(str, "%08x%08x", block.MS, block.LS);
}
