
#include <vector>

#ifndef BTREE_H
#define BTREE_H
#include "BTREE.h"

node* BTree::initNode() {
    node* n = new node;
    n->nKeys = 0;
    n->isLeaf = true;

    return n;
}


BTree::BTree(int branchFactor) {
    size = branchFactor;
    node* x = initNode();
    root = x;
}


BTree::~BTree() {
    cleanup(root);
}


void BTree::insert(int k) {
    node* r = root;
    if(r->nKeys == 2 * size - 1) { // if root is full, split node, then insert

        node* s = initNode();
        root = s;
        s->isLeaf = false;
        s->children.push_back(r);
        // splitChild(s, 1, r);
        insertNonfull(s, k);
    }
    else {
        insertNonfull(r, k);
    }
}

void BTree::insertNonfull(node* x, int k) {
    
    int i = x->nKeys;

    auto riter = x->keys.rbegin();
    for(; riter != x->keys.rend() && k < (*riter); riter++); // find the position to insert at

    if(x->isLeaf) {
        
        x->keys.insert(riter.base(), k);
        x->nKeys++;
    }
    else {

        int i = riter.base() - x->keys.begin() + 1;
        node* c = x->children[i];

        if(c->nKeys == 2 * size - 1) {
            
            // splitChild(x, i, c)

            if(k > x->keys[i]) i++;
        }

        insertNonfull(c, k);
    }
}


void BTree::splitChild(node* x, int i, node* y) {

    node* z = initNode();
    x->isLeaf = y->isLeaf;
    z->nKeys = size - 1;
    int j;

    for(j = 1; j < size - 1; j++) { // copy y keys to z

        z->keys[j] = y->keys[j + size];
    }

    if(!y->isLeaf) { // put y children in z

        for(j = 1; j < size; j++) {

            z->children[j] = y->children[j + size];
        }
    }

    y->nKeys = size - 1;

    x->children[i + 1] = z;
    x->keys[i] = y->keys[size];
    x->nKeys++;
}


bool BTree::search(node* x, int k) {

    int i = 1;

    for(; i <= x->nKeys && k >= x->keys[i]; i++);

    if(i <= x->nKeys && k == x->keys[i]) return true;

    if(x->isLeaf) return false;
    
    search(x->children[i], k);
}





#endif