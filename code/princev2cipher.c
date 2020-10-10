/**
princev2cipher.c

This program uses a 128-bit key to encrypt or decrypt a 64-bit text

Sample Usage:

To encrypt:
> princev2cipher E k0 k1 m

To decrypt:
> princev2cipher D k0 k1 c

Sample build:
> make princev2cipher
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key.h"
#include "princev2.h"

enum{ENCRYPT = 0, DECRYPT = 1};

int main(int argc, char* argv[]) {
    // check number of arguments
    if (argc != 5) {
        fprintf(stderr,
                "Usage: %s {E/D} k0 k1 m\t# encrypt/decrypt with 128-bit key\n",
                argv[0]);
        return -1;
    }

    // parse encryption/decryption flag
    int mode;
    if (!strcmp(argv[1], "E")) {
        mode = ENCRYPT;
    } else if (!strcmp(argv[1], "D")) {
        mode = DECRYPT;
    } else {
        fprintf(stderr,
                "%s: incorrect usage\n(to encrypt) %s E k0 k1 m\n(to decrypt) %s D k0 k1 c\n",
                argv[0], argv[0], argv[0]);
        return -1;
    }

    // parse keys
    uint64_t k0 = 0;
    uint64_t k1 = 0;

    char *checkptr;
    if (argc == 5) {
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

    // parse input block
    uint64_t m = strtoul(argv[argc-1], &checkptr, 16);
    if (*checkptr != '\0') {
        fprintf(stderr, "failed to parse m = %s from %s on\n", argv[argc-1], checkptr);
        return -1;
    }

    uint64_t c;
    if (mode == ENCRYPT) {
        c = prince_encrypt(key_new(k0, k1), m);
    } else {
        c = prince_decrypt(key_new(k0, k1), m);
    }

    printf("%016lx\n", c);

    return 0;
}
