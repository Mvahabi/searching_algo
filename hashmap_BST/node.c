#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cite: Christian helped with node create via office hours

// Constructs a new node
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->left = NULL;
        n->right = NULL;
        if (oldspeak != NULL) {
            n->oldspeak = strdup(oldspeak);
            if (!n->oldspeak) {
                n->oldspeak = NULL;
            }
        } else {
            free(n->oldspeak);
        }

        if (newspeak != NULL) {
            n->newspeak = strdup(newspeak);
            if (!n->oldspeak) {
                free(n->oldspeak);
            }
        } else {
            n->newspeak = NULL;
        }
    }
    return n;
}

// Destructor for a node
void node_delete(Node **n) {
    if (*n) {
        free((*n)->newspeak);
        (*n)->newspeak = NULL;
        free((*n)->oldspeak);
        (*n)->oldspeak = NULL;
        free(*n);
        *n = NULL;
    }
    return;
}

// Prints the node
void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak && !n->newspeak) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
