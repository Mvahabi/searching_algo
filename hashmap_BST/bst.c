#include "bst.h"

#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Citation: mostly from Erik's slides, Eugene's video, as well as Professor Long's lecture 18 slides

// Constructor for bst
Node *bst_create(void) {
    return NULL;
}

// Destructor for bst
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
    return;
}

// conditional statement to check if x is greater than y
static uint32_t max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}

// Returns the height of bst rooted at root
uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    } else {
        return (1 + max(bst_height(root->left), bst_height(root->right)));
    }
}

// Returns the number of nodes in the bst
uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    return bst_size(root->left) + bst_size(root->right) + 1;
}

// Searches for a node containing oldspeak in bst rotted at root
Node *bst_find(Node *root, char *oldspeak) {
    Node *cur = root;
    if (root && oldspeak) {
        while (cur && strcmp(cur->oldspeak, oldspeak) != 0) {
            if (strcmp(cur->oldspeak, oldspeak) > 0) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
    }
    return cur;
}

// Inserts a new node into the bst rooted at root
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    Node *x = root;
    Node *y = NULL;
    if (root != NULL && oldspeak != NULL) {
        while (x != NULL) {
            y = x;
            if (strcmp(x->oldspeak, oldspeak) >= 0) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        if (strcmp(y->oldspeak, oldspeak) >= 0) {
            y->left = node_create(oldspeak, newspeak);
        } else {
            y->right = node_create(oldspeak, newspeak);
        }
    } else {
        y = node_create(oldspeak, newspeak);
    }

    return y;
}

// Prints out each node in the binary search tree through an inorder traversal
void bst_print(Node *root) {
    for (Node *n = root->left; n != root->right; n = n->left) {
        node_print(n);
    }
    return;
}
