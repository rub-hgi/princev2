/**
key.c

Implementation for expanded key used by PRINCEv2 cipher
**/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key.h"
#include "misc.h"

const uint64_t ALPHA = 0xc0ac29b7c97c50dd;
const uint64_t BETA  = 0x3f84d5b5b5470917;

const uint64_t RCs[] = {
    0x0000000000000000,
        0x13198a2e03707344,                         // RC 1
            0xa4093822299f31d0,                     // RC 2
                0x082efa98ec4e6c89,                 // RC 3
                    0x452821e638d01377,             // RC 4
                        0xbe5466cf34e90c6c,         // RC 5
                        0xbe5466cf34e90c6c ^ ALPHA, // RC 6 = RC5 + alpha
                    0x452821e638d01377 ^ BETA,      // RC 7 = RC4 + beta
                0x082efa98ec4e6c89 ^ ALPHA,         // RC 8 = RC3 + alpha
            0xa4093822299f31d0 ^ BETA,              // RC 9 = RC2 + beta
        0x13198a2e03707344 ^ ALPHA,                 // RC10 = RC1 + alpha
};

/* creates new princev2key_t from 128 bits */
princev2key_t key_new(uint64_t k0, uint64_t k1) {
    princev2key_t oKey = {
        .k0 = k0,
        .k1 = k1,
    };
    return oKey;
}

/* creates new princev2key_t from random values */
princev2key_t key_newRandom() {
    return key_new(llrand(), llrand());
}

/* creates new princev2key_t from 48 character string */
princev2key_t key_newFromString(char keyString[KEY_HEX_LENGTH + 1]) {
    uint64_t value[2];

    int valueIndex;
    int placeIndex;
    int stringIndex = 0;

    assert(strlen(keyString) == KEY_HEX_LENGTH);

    for (valueIndex = 0; valueIndex < 2; valueIndex++) {
        value[valueIndex] = 0;
        for (placeIndex = 15; placeIndex >= 0; placeIndex--) {
            uint64_t num = (uint64_t) getInt(keyString[stringIndex]);
            value[valueIndex] +=  (num << 4*placeIndex);
            stringIndex++;
        }
    }

    return key_new(value[0], value[1]);
}


/* sets key to random values */
void key_getRandom(princev2key_t* key) {
    key->k0 = llrand();
    key->k1 = llrand();
}

/* prints expanded key */
void key_printExpanded(princev2key_t key) {
    printf("k0 : %016lx\n", key.k0);
    printf("k1 : %016lx\n", key.k1);
}

/* prints key */
void key_print(princev2key_t key) {
    printf("%016lx%016lx\n", key.k0, key.k1);
}

/* creates string str for key */
void princev2key_toString(princev2key_t key, char str[KEY_HEX_LENGTH + 1]) {
    sprintf(str, "%016lx%016lx", key.k0, key.k1);
}
