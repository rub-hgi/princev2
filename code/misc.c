#include <stdio.h>
#include <stdlib.h>

#include "misc.h"

/* generate a 64bit random unsigned int */
uint64_t llrand() {
    uint64_t r = 0;

    for (ssize_t i = 0; i < 5; ++i) {
        r = (r << 15) | (rand() & 0x7FFF);
    }

    return r & 0xFFFFFFFFFFFFFFFFULL;
}

int getInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >='a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    fprintf(stderr, "getInt: invalid character! must be 0-9 or a-f");
    exit(-1);
}
