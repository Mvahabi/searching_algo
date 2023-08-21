#include "ht.h"
#include "salts.h"
#include "bst.h"
#include "node.h"

#include "speck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Citation: Erik's psuedocode

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Creates a new hash table
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// Deletes the hash table
void ht_delete(HashTable **ht) {
    if (*ht) {
        for (uint32_t i = 0; i < ht_size((*ht)); i++) {
            if ((*ht)->trees[i]) {
                bst_delete(&(*ht)->trees[i]);
            }
        }
        free((*ht)->trees);
        (*ht)->trees = NULL;
        free((*ht));
        *ht = NULL;
    }
    return;
}

// Returns the size of the hash table
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Checks if a oldspeak word is in the hash table
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t i = hash(ht->salt, oldspeak) % ht->size;
    return bst_find(ht->trees[i], oldspeak);
}

// Inserts a oldspeak, newspeak pair into the hash table
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t i = hash(ht->salt, oldspeak) % ht->size;
    ht->trees[i] = bst_insert(ht->trees[i], oldspeak, newspeak);
}

// Counts the number of initialized linked lists
uint32_t ht_count(HashTable *ht) {
    uint32_t x = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            x++;
        }
    }
    return x;
}

// Returns the average binary search tree size
double ht_avg_bst_size(HashTable *ht) {
    uint32_t avg_size = 0;
    for (uint32_t i = 0; ht_size(ht); i++) {
        avg_size += bst_size(ht->trees[i]);
    }
    return (double) (avg_size / ht_count(ht));
}

// Returns the average binary search tree height
double ht_avg_bst_height(HashTable *ht) {
    uint32_t avg_height = 0;
    for (uint32_t i = 0; ht->size; i++) {
        avg_height += bst_height(ht->trees[i]);
    }
    return (double) (avg_height / ht_count(ht));
}

// Prints the hash table
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            bst_print(ht->trees[i]);
        }
    }
    return;
}
