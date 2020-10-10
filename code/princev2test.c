/**
princetest.c

This program generates N random plaintext and finds the the corresponding
ciphertext. The key can either be random (different key for all N plaintexts)
or fixed (same key for all N plaintext)

Sample Usage:

Random Keys:
> princev2test 10

Fixed Key
> princev2test 10 0123456789abcdef 0123456789abcdef 0123456789abcdef

Sample build:
> make princev2test
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "key.h"
#include "misc.h"
#include "princev2.h"

enum{FIXED_KEY = 0, RANDOM_KEY = 1};

int main(int argc, char* argv[]) {
    // seed random number generator
    srand(time(NULL));

    int mode; // whether key is FIXED_KEY or RANDOM_KEY
    if (argc == 2) {
        mode = RANDOM_KEY;
    } else if (argc == 4) {
        mode = FIXED_KEY;
    } else {
        fprintf(stderr,
                "Usage: %s <Number_of_tests> [k0 k1]\n",
            argv[0]);
        return -1;
    }

    int num = atoi(argv[1]);
    if (num <= 0) {
        return 0;
    }

    // print round constants
    for (ssize_t i = 1; i < 11; i++) {
        printf("RC[%2lu] = %016lx\n", i, RCs[i]);
    }
    printf("\nalpha = %016lx\n", ALPHA);
    printf("beta  = %016lx\n\n", BETA);

    // testvectors
    printf("Key                              Plaintext        Ciphertext       decrypted CT\n");
    uint64_t p, k0, k1, c, ptest;
    k0 = 0x0000000000000000;
    k1 = 0x0000000000000000;
    p = 0x0000000000000000;
    c = prince_encrypt(key_new(k0, k1), p);
    ptest = prince_decrypt(key_new(k0, k1), c);
    printf("%016lx%016lx %016lx %016lx %016lx\n", k0, k1, p, c, ptest);

    k0 = 0xffffffffffffffff;
    k1 = 0x0000000000000000;
    p = 0x0000000000000000;
    c = prince_encrypt(key_new(k0, k1), p);
    ptest = prince_decrypt(key_new(k0, k1), c);
    printf("%016lx%016lx %016lx %016lx %016lx\n", k0, k1, p, c, ptest);

    k0 = 0x0000000000000000;
    k1 = 0xffffffffffffffff;
    p = 0x0000000000000000;
    c = prince_encrypt(key_new(k0, k1), p);
    ptest = prince_decrypt(key_new(k0, k1), c);
    printf("%016lx%016lx %016lx %016lx %016lx\n", k0, k1, p, c, ptest);

    k0 = 0x0000000000000000;
    k1 = 0x0000000000000000;
    p = 0xffffffffffffffff;
    c = prince_encrypt(key_new(k0, k1), p);
    ptest = prince_decrypt(key_new(k0, k1), c);
    printf("%016lx%016lx %016lx %016lx %016lx\n", k0, k1, p, c, ptest);

    p = 0x0123456789abcdef;
    k0 = 0x0123456789abcdef;
    k1 = 0xfedcba9876543210;
    c = prince_encrypt(key_new(k0, k1), p);
    ptest = prince_decrypt(key_new(k0, k1), c);
    printf("%016lx%016lx %016lx %016lx %016lx\n", k0, k1, p, c, ptest);

    printf("\n");

    // process key
    k0 = 0;
    k1 = 0;
    if (mode == FIXED_KEY) {
        char *checkptr;
        if (argc == 4) {
            k0 = strtoul(argv[2], &checkptr, 16);
            if (*checkptr != '\0') {
                fprintf(stderr, "failed to parse k0 = %s from %s on\n", argv[2], checkptr);
                return -1;
            }

            k1 = strtoul(argv[3], &checkptr, 16);
            if (*checkptr != '\0') {
                fprintf(stderr, "failed to parse k1 = %s from %s on\n", argv[3], checkptr);
                return -1;
            }
        }
    } else {
        k0 = llrand();
        k1 = llrand();
    }

    princev2key_t key = key_new(k0, k1);
    char keyStr[KEY_HEX_LENGTH+1];

    if (mode == FIXED_KEY) {
        princev2key_toString(key, keyStr);
        printf("Key: %s\n", keyStr);
    } else {
        printf("Key                              ");
    }
    printf("Plaintext        Ciphertext       decrypted CT\n");

    // create test vectors
    for (int i = 0; i < num; i++) {
        // initialize
        uint64_t plaintext = llrand();
        uint64_t ciphertext = prince_encrypt(key, plaintext);
        uint64_t plaintext_test = prince_decrypt(key, ciphertext);

        if (mode == RANDOM_KEY) {
            princev2key_toString(key, keyStr);
            printf ("%s ", keyStr);

            key_getRandom(&key);
        }
        printf("%016lx %016lx %016lx\n", plaintext, ciphertext, plaintext_test);
    }
}
