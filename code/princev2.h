#ifndef _PRINCE_
#define _PRINCE_

#include "key.h"

/* SBOX_SIZE must be equivalent to 2 ** NIBBLE_SIZE */
enum {SBOX_SIZE = 16};
enum {NUM_OF_ROUNDS = 12};
typedef enum {ENC, DEC} princemode_t;

extern const char prince_sbox[];
extern const char prince_sbox_inverse[];

uint64_t prince_s_layer(uint64_t state, const char sbox[SBOX_SIZE]);
uint64_t prince_m_layer(uint64_t state);
uint64_t prince_shiftRow(uint64_t state);
uint64_t prince_roundForward(uint64_t k1, uint64_t state, uint64_t RCi);
uint64_t prince_roundInverse(uint64_t k1, uint64_t state, uint64_t RCi);
uint64_t prince_core(princev2key_t key, uint64_t state, princemode_t dec);
uint64_t prince_encrypt(princev2key_t key, uint64_t plaintext);
uint64_t prince_decrypt(princev2key_t key, uint64_t ciphertext);

#endif

