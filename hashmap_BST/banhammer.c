#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "parser.h"
#include "bf.h"
#include "bst.h"
#include "ht.h"
#include "messages.h"
#include "node.h"
#include "parser.h"

// Citation: Erik's pseudocode
#define WORDS   "[a-zA-Z0-9_]+(('|-)[[a-zA-Z0-9_]+)*" // uses OR operation
#define OPTIONS "t:f:sh"

int main(int argc, char **argv) {
    // Initializing needed variables
    int opt = 0;
    bool help = false;
    bool stats = false;

    uint32_t s_ht = (uint32_t) pow(2, 16);
    uint32_t s_bf = (uint32_t) pow(2, 20);
    uint64_t branches = 0;
    uint64_t lookups = 0;

    // command line functuin
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 's': stats = true; break;
        case 't': s_ht = (uint32_t) optarg; break;
        case 'f': s_bf = (uint32_t) optarg; break;
        }
    }

    // help function
    if (help) {
        printf("SYNOPSIS\n");
        printf("  A word filtering program for the GPRSC.\n");
        printf("  Filters out and reports bad words parsed from stdin.\n");
        printf("\nUSAGE\n");
        printf("  ./banhammer [-hs] [-t size] [-f size]\n");
        printf("\nOPTIONS\n");
        printf("  -h           Program usage and help.\n");
        printf("  -s           Print program statistics.\n");
        printf("  -t size      Specify hash table size (default: 2^16).\n");
        printf("  -f size      Specify Bloom filter size (default: 2^20).\n");
        printf("\n");
        return 0;
    }

    // checking if user entered valid hash table size then create it
    if (s_ht <= 0) {
        fprintf(stderr, "\nFailed to create Hash Table.\n");
        exit(1);
    }

    HashTable *ht = ht_create(s_ht);

    // checking if user entered valid bloom filter size then create it
    if (s_bf <= 0) {
        fprintf(stderr, "\nFailed to crate Bloom Filter.\n");
        exit(1);
    }

    BloomFilter *bf = bf_create(s_bf);

    // create the badspeak text file
    char *badspeak = NULL;
    FILE *bad_file = fopen("badspeak.txt", "w");
    if (bad_file == NULL) {
        fprintf(stderr, "NO FILE EXISTS!\n");
    }
    while (fscanf(bad_file, "%s\n", badspeak) != EOF) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }
    fclose(bad_file);

    // create a newspeak text file
    char *oldspeak = NULL;
    char *newspeak = NULL;
    FILE *new_file = fopen("newspeak.txt", "w");
    if (new_file == NULL) {
        fprintf(stderr, "NO FILE EXISTS!\n");
    }
    while (fscanf(new_file, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }
    fclose(new_file);

    // regex implementation
    regex_t reg;
    if (regcomp(&reg, WORDS, REG_EXTENDED)) {
        fprintf(stderr, "\nERROR!\n");
        return 1;
    }

    Node *mixed_msg = bst_create();
    Node *bad_msg = bst_create();

    char *word = NULL;
    // For each word that is read in, check to see if it has been added to the Bloom filter.
    while ((word = next_word(stdin, &reg))) {
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        if (bf_probe(bf, word)) {
            Node *n = ht_lookup(ht, word);
            lookups++;
            if (n) {
                if (n->oldspeak && n->newspeak) {
                    mixed_msg = bst_insert(mixed_msg, n->oldspeak, n->newspeak);
                    branches++;
                } else if (n->oldspeak) {
                    bad_msg = bst_insert(bad_msg, n->oldspeak, NULL);
                    branches++;
                }
            }
        }
    }

    // prints out the statistic
    if (stats) {
        fprintf(stdout, "Average bst size: %f\n", ht_avg_bst_size(ht));
        fprintf(stdout, "Average bst height: %f\n", ht_avg_bst_height(ht));
        fprintf(stdout, "Average bst tree traversal: %lu\n", ((uint64_t) branches / lookups));
        fprintf(
            stdout, "Hash table load: %0.6u%%\n", 100 * ((uint32_t) ht_count(ht) / ht_size(ht)));
        fprintf(
            stdout, "Bloom filter load: %0.6u%%\n", 100 * ((uint32_t) bf_count(bf) / bf_size(bf)));
    } else {
        if (bst_size(mixed_msg) > 0 && bst_size(bad_msg) == 0) {
            printf("%s", goodspeak_message);
            bst_print(mixed_msg);
        } else if (bst_size(mixed_msg) == 0 && bst_size(bad_msg) > 0) {
            printf("%s", mixspeak_message);
            bst_print(bad_msg);
            bst_print(mixed_msg);
        } else if (bst_size(mixed_msg) == 0 && bst_size(bad_msg) > 0) {
            printf("%s", badspeak_message);
            bst_print(bad_msg);
        }
    }

    // clears and empties any memory spaces
    bst_delete(&mixed_msg);
    bst_delete(&bad_msg);
    bf_delete(&bf);
    ht_delete(&ht);
    clear_words();
    regfree(&reg);
    return 0;
}
