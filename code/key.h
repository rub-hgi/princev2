/**
key.h

Interface for expanded key used by PRINCEv2 cipher
**/

#ifndef _KEY_INCLUDED_
#define _KEY_INCLUDED_

#ifndef BIT_KEY_LENGTH
#define BIT_KEY_LENGTH 128
#endif

#include <inttypes.h>

/* length of non-expanded key expressed in hex*/
enum{KEY_HEX_LENGTH = BIT_KEY_LENGTH/4};

/* each of the five 64-bits parts are stored as an uint64_t*/
typedef struct key {
    uint64_t k0;
    uint64_t k1;
} princev2key_t;


/* round constants */
extern const uint64_t ALPHA;
extern const uint64_t BETA;

extern const uint64_t RCs[];

/* creates new princev2key_t from 128 bits */
princev2key_t key_new(uint64_t k0, uint64_t k1);

/* creates new princev2key_t from random values */
princev2key_t key_newRandom();

/* creates new princev2key_t from 48 character string */
princev2key_t key_newFromString(char keyString[KEY_HEX_LENGTH + 1]);

/* sets key to random values */
void key_getRandom(princev2key_t* key);

/* prints expanded key */
void key_printExpanded(princev2key_t key);

/* prints key */
void key_print(princev2key_t key);

/* creates string str for key */
void princev2key_toString(princev2key_t key, char str[KEY_HEX_LENGTH + 1]);

#endif
