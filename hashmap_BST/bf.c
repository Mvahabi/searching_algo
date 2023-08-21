#include "bf.h"

#include "bv.h"
#include "salts.h"
#include "speck.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Citation: Erik the homie helped with most of these functions

struct BloomFilter {
    uint64_t prim[2];
    uint64_t sec[2];
    uint64_t ter[2];
    BitVector *fil;
};

// Constructor for bf
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));

    bf->prim[0] = SALT_PRIMARY_LO;
    bf->prim[1] = SALT_PRIMARY_HI;

    bf->sec[0] = SALT_SECONDARY_LO;
    bf->sec[1] = SALT_SECONDARY_HI;

    bf->ter[0] = SALT_TERTIARY_LO;
    bf->ter[1] = SALT_TERTIARY_HI;
    bf->fil = bv_create(size);

    return bf;
}

// Destructor for bf
void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->fil);
    free(*bf);
    *bf = NULL;
    return;
}

// bf length is returned as size
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->fil);
}

// Takes oldspeak and inserts it into the bf
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->fil, hash(bf->prim, oldspeak) % bf_size(bf));
    bv_set_bit(bf->fil, hash(bf->sec, oldspeak) % bf_size(bf));
    bv_set_bit(bf->fil, hash(bf->ter, oldspeak) % bf_size(bf));
    return;
}

// Probes the bf for oldspeak
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    if (bv_get_bit(bf->fil, hash(bf->prim, oldspeak) % bf_size(bf))
        && bv_get_bit(bf->fil, hash(bf->sec, oldspeak) % bf_size(bf))
        && bv_get_bit(bf->fil, hash(bf->ter, oldspeak) % bf_size(bf))) {
        return true;
    }
    return false;
}

// returns the number of set bits
uint32_t bf_count(BloomFilter *bf) {
    uint64_t x = 0;
    for (uint32_t i = 0; i < bv_length(bf->fil); i++) { // Loop through the filter
        x += bv_get_bit(bf->fil, i);
    }
    return x / 8;
}

// Prints the bits of bf
void bf_print(BloomFilter *bf) {
    bv_print(bf->fil);
    return;
}
