#include "bv.h"

#include <stdio.h>
#include <stdlib.h>

// Code taken from assignment 5, code.c

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Code taken from Eugene's previous byte implementation
static inline size_t bytes(size_t bits) {
    return bits % 8 ? bits / 8 + 1 : bits / 8;
}

// constructor for bv
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc(bytes(bv->length), sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

// destructor for bv
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        (*bv)->vector = NULL;
        free(*bv);
        *bv = NULL;
    }
    return;
}

// length of bv is returned
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// set bit in the bv.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i < bv_length(bv)) {
        bv->vector[i / 8] = 1;
        return true;
    }
    return false;
}

// clear bit from bv.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i < bv_length(bv)) {
        bv->vector[i / 8] = 0;
        return true;
    }
    return false;
}

// get a bit from the bit vector
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i < bv_length(bv) || bv->vector[i / 8] != 0) {
        return bv->vector[i / 8] == 1;
    }
    return false;
}

// Debugger function to print bit vector
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i++) {
        printf("[%d]", bv_get_bit(bv, i) ? 1 : 0);
    }
    printf("\n");
}
