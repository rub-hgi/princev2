/**
princev2.c

Implementation for PRINCEv2 cipher
**/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "princev2.h"
#include "block.h"

/*
   each of the values below store the binary values along the diagonal of
   matrix M0, M1, M2, and M3. We only use the last 4 bits of the integer

   for instance,

   M0 =
   0000
   0100
   0010
   0001

   The diagonal is 0111 which is binary for 14, so m0 = 14
   */
enum{m0 = 7, m1 = 11, m2 = 13, m3 = 14};

const char prince_shift[NUM_OF_NIBBLES] = {
     0,  5, 10, 15,
     4,  9, 14,  3,
     8, 13,  2,  7,
    12,  1,  6, 11
};

const char prince_shift_inverse[NUM_OF_NIBBLES] = {
     0, 13, 10,  7,
     4,  1, 14, 11,
     8,  5,  2, 15,
    12,  9,  6,  3
};

const char prince_sbox[] = {
    0xb, 0xf, 0x3, 0x2,
    0xa, 0xc, 0x9, 0x1,
    0x6, 0x7, 0x8, 0x0,
    0xe, 0x5, 0xd, 0x4
};

const char prince_sbox_inverse[] = {
    0xb, 0x7, 0x3, 0x2,
    0xf, 0xd, 0x8, 0x9,
    0xa, 0x6, 0x4, 0x0,
    0x5, 0xe, 0xc, 0x1
};

uint64_t prince_s_layer(uint64_t iState, const char sbox[SBOX_SIZE]) {
    uint64_t oState = 0;

    for (uint64_t i = 0; i < NUM_OF_NIBBLES; i++) {
        uint8_t nibble = (iState >> (4*i)) & 0xf;
        oState |= ((uint64_t) sbox[nibble]) << (4*i);
    }

    return oState;
}

/* multiplies matrix MHat0 by 4 elements of state beginning at starting index
   MHat0 =
   M0 M1 M2 M3
   M1 M2 M3 M0
   M2 M3 M0 M1
   M3 M0 M1 M2
   */
static void prince_MHat0Multiply(uint64_t *state, int startingIndex ) {
    block_t state_block = {*state >> 32, *state & 0xffffffff};

    int nibble0 = block_getNibble(state_block, startingIndex);
    int nibble1 = block_getNibble(state_block, startingIndex + 1);
    int nibble2 = block_getNibble(state_block, startingIndex + 2);
    int nibble3 = block_getNibble(state_block, startingIndex + 3);

    block_setNibble(&state_block, startingIndex,
                    (m0 & nibble0) ^ (m1 & nibble1) ^ (m2 & nibble2) ^ (m3 & nibble3));
    block_setNibble(&state_block, startingIndex + 1,
                    (m1 & nibble0) ^ (m2 & nibble1) ^ (m3 & nibble2) ^ (m0 & nibble3));
    block_setNibble(&state_block, startingIndex + 2,
                    (m2 & nibble0) ^ (m3 & nibble1) ^ (m0 & nibble2) ^ (m1 & nibble3));
    block_setNibble(&state_block, startingIndex + 3,
                    (m3 & nibble0) ^ (m0 & nibble1) ^ (m1 & nibble2) ^ (m2 & nibble3));

    *state = ((uint64_t) state_block.MS << 32) | (uint64_t) state_block.LS;
}

/* multiplies matrix MHat1 by 4 elements of state beginning at starting index
   MHat1 =
   M1 M2 M3 M0
   M2 M3 M0 M1
   M3 M0 M1 M2
   M0 M1 M2 M3

*/
static void prince_MHat1Multiply(uint64_t* state, int startingIndex ) {
    block_t state_block = {*state >> 32, *state & 0xffffffff};

    int nibble0 = block_getNibble(state_block, startingIndex);
    int nibble1 = block_getNibble(state_block, startingIndex + 1);
    int nibble2 = block_getNibble(state_block, startingIndex + 2);
    int nibble3 = block_getNibble(state_block, startingIndex + 3);

    block_setNibble(&state_block, startingIndex,
                    (m1 & nibble0) ^ (m2 & nibble1) ^ (m3 & nibble2) ^ (m0 & nibble3));
    block_setNibble(&state_block, startingIndex + 1,
                    (m2 & nibble0) ^ (m3 & nibble1) ^ (m0 & nibble2) ^ (m1 & nibble3));
    block_setNibble(&state_block, startingIndex + 2,
                    (m3 & nibble0) ^ (m0 & nibble1) ^ (m1 & nibble2) ^ (m2 & nibble3));
    block_setNibble(&state_block, startingIndex + 3,
                    (m0 & nibble0) ^ (m1 & nibble1) ^ (m2 & nibble2) ^ (m3 & nibble3));

    *state = ((uint64_t) state_block.MS << 32) | (uint64_t) state_block.LS;
}

/* state becomes state multiplied by specified matrix M

   multiply by

   M^0 - - -
   - M^1 - -
   - - M^1 -
   - - - M^0
   */
uint64_t prince_m_layer(uint64_t state) {
    prince_MHat0Multiply(&state, 0);
    prince_MHat1Multiply(&state, 4);
    prince_MHat1Multiply(&state, 8);
    prince_MHat0Multiply(&state, 12);

    return state;
}

uint64_t prince_permuteNibbles(uint64_t state, const char shift[NUM_OF_NIBBLES]) {
    block_t originalState = {state >> 32, state & 0xffffffff};
    block_t state_block = {state >> 32, state & 0xffffffff};

    for (ssize_t i = 0; i < NUM_OF_NIBBLES; i++) {
        uint64_t nibble = block_getNibble(originalState, shift[i]);
        block_setNibble(&state_block, i, nibble);
    }

    return ((uint64_t) state_block.MS << 32) | (uint64_t) state_block.LS;
}

uint64_t prince_shiftRow(uint64_t state) {
    return prince_permuteNibbles(state, prince_shift);
}

uint64_t prince_shiftRowInverse(uint64_t state) {
    return prince_permuteNibbles(state, prince_shift_inverse);
}


uint64_t prince_roundForward(uint64_t state, uint64_t rk, uint64_t RCi) {
    state = prince_s_layer(state, prince_sbox);
    state = prince_m_layer(state);
    state = prince_shiftRow(state);

    state ^= RCi;
    state ^= rk;

    return state;
}

uint64_t prince_roundInverse(uint64_t state, uint64_t rk, uint64_t RCi) {
    state ^= rk;
    state ^= RCi;

    state = prince_shiftRowInverse(state);
    state = prince_m_layer(state);
    state = prince_s_layer(state, prince_sbox_inverse);

    return state;
}

uint64_t prince_core(princev2key_t key, uint64_t state, princemode_t mode) {
    uint64_t rkeys[] = {key.k0, key.k1};
    state ^= rkeys[0];

    for (ssize_t i = 1; i < NUM_OF_ROUNDS / 2; i++) {
        state = prince_roundForward(state, rkeys[i % 2], RCs[i]);
    }

    state = prince_s_layer(state, prince_sbox);
    state ^= rkeys[0];
    state = prince_m_layer(state);

    if (mode == DEC) {
        rkeys[0] ^= ALPHA ^ BETA;
        rkeys[1] ^= ALPHA ^ BETA;
    }

    state ^= rkeys[1] ^ BETA;
    state = prince_s_layer(state, prince_sbox_inverse);

    for (ssize_t i = NUM_OF_ROUNDS / 2; i < NUM_OF_ROUNDS - 1; i++) {
        state = prince_roundInverse(state, rkeys[i % 2], RCs[i]);
    }

    state ^= rkeys[1] ^ BETA;

    return state;
}

uint64_t prince_encrypt(princev2key_t key, uint64_t plaintext) {
    return prince_core(key, plaintext, ENC);
}

uint64_t prince_decrypt(princev2key_t key, uint64_t ciphertext) {
    return prince_core(key_new(key.k1^BETA, key.k0^ALPHA), ciphertext, DEC);
}
