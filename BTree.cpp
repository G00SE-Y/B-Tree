#include <iostream>
#include <vector>

#ifndef BTREE_H
#define BTREE_H
#include "BTREE.h"


// getters
int BTree::getsize() { return size; }
int BTree::getKeyCount() { return keyCount; }
int BTree::getnodeCount() { return nodeCount; }
int BTree::getHeight() { return height; }


node* BTree::initNode() {
    node* n = new node;
    nodeCount++;
    n->nKeys = 0;
    n->isLeaf = true;
    return n;
}


BTree::BTree(int branchFactor) {
    size = branchFactor;
    keyCount = 0;
    nodeCount = 0;
    height = 1;

    node* x = initNode();
    root = x;
}


BTree::~BTree() {
    cleanup(root);
}


void BTree::cleanup(node* r) {

    for(auto c: r->children) { // recurse to leaf
        cleanup(c);
    }
    delete r; // delete current node
}


void BTree::insert(int k) {
    
    node* r = root;
    if(r->nKeys == 2 * size - 1) { // if root is full, split node, then insert

        node* s = initNode(); // init new node
        
        root = s;
        s->isLeaf = false;
        s->children.push_back(r);

        splitChild(s, 0, r); // split root
        height++;
        insertNonfull(s, k);
    }
    else {
        insertNonfull(r, k);
    }
    keyCount++;
}


void BTree::insertNonfull(node* x, int k) {
    
    if(x->nKeys == 0 && x == root) { // if tree is completely empty, just insert
        
        x->keys.push_back(k);
        x->nKeys++;
    }
    else {

        auto iter = x->keys.begin();
        auto stop = x->keys.end();
        int i = 0;

        while(iter != stop && *iter < k) { // traverse
            iter++;
            i++;
        }

        // prev while loop will go +1 past the end of the keys
        i--;
        iter--;

        if(x->isLeaf) {
            
            if(*iter < k) {
                x->keys.push_back(k);
            }
            else {
                x->keys.insert(iter, k);
            }
            x->nKeys++;
        }
        else {
            
            if(*iter < k) i++;

            if(x->children[i]->nKeys == 2 * size - 1) {
                
                splitChild(x, i, x->children[i]);
                if(x->keys[i] < k) i++;
            }

            insertNonfull(x->children[i], k);
        }
    }
}


void BTree::splitChild(node* x, int i, node* y) {

    node* z = initNode();
    z->isLeaf = y->isLeaf;
    z->nKeys = size - 1;
    
    int j;
    for(j = (2 * size - 1) - 1; j > size - 1; j--) { // copy y keys to z
        z->keys.insert(z->keys.begin(), y->keys[j]);
        y->keys.pop_back();
    }

    if(!y->isLeaf) { // if y is internal, then children need to be moved as well

        for(j = (2 * size - 1); j > size - 1; j--) { // copy y's children in z
            z->children.insert(z->children.begin(), y->children[j]);
            y->children.pop_back();
        }
    }

    y->nKeys = size - 1;
    x->children.insert(x->children.begin() + i + 1, z); // insert z into x, next to y
    x->keys.insert(x->keys.begin() + i, y->keys.back());
    x->nKeys++;
    y->keys.pop_back();
}


bool BTree::search(node* x, int k) {

    int i = 1;

    for(; i <= x->nKeys && k >= x->keys[i]; i++);

    if(i <= x->nKeys && k == x->keys[i]) return true;

    if(x->isLeaf) return false;
    
    return search(x->children[i], k);
}


void BTree::printInOrder() {

    std::cout << "BTREE:" << std::endl;
    if(root->nKeys == 0) {
        std::cout << "Empty." << std::endl;
    }
    else {
        printHelper(root);
        std::cout << std::endl;
    }
    std::cout << "Total Keys in tree: " << keyCount << std::endl;
    std::cout << "Height of tree: " << height << std::endl;
    std::cout << "Total Nodes in tree: " << nodeCount << std::endl;
}


void BTree::printHelper(node* n) {
    
    int keys = n->nKeys;

    if(n->children.size() != 0) { // leaf node

        printHelper(n->children[0]);
        for(int i = 1; i < n->children.size(); i++) {
            std::cout << n->keys[i - 1] << " ";
            printHelper(n->children[i]);
        }
    }
    else {
        for(int val: n->keys) {
            std::cout << val << " ";
        }
    }
}


void BTree::printNodes() {
    
    std::cout << "BTREE (Preorder traversal):" << std::endl;
    if(root->nKeys == 0) {
        std::cout << "Empty." << std::endl;
    }
    else {
        printNodesHelper(root, 0);
        std::cout << std::endl;
    }
    std::cout << "Total Keys in tree: " << keyCount << std::endl;
    std::cout << "Height of tree: " << height << std::endl;
    std::cout << "Total Nodes in tree: " << nodeCount << std::endl;
}


void BTree::printNodesHelper(node* n, int depth) {

    std::cout << "{depth: " << depth << ", keys: " << n->keys.size() << ", children: " << n->children.size() << "} keys: [";

    for(int i = 0; i < n->nKeys - 1; i++) {
        std::cout << n->keys[i] << ", ";
    }
    std::cout << n->keys.back() << "]" << std::endl;

    for(auto c: n->children) {
        printNodesHelper(c, depth + 1);
    }
}


std::vector<int> BTree::getKeys() {

    std::vector<int> keys;

    if(root->nKeys != 0) {

        auto temp = getKeysHelper(root);
        keys.insert(keys.end(), temp.begin(), temp.end());
    }

    return keys;
}

std::vector<int> BTree::getKeysHelper(node* n) {
    
    std::vector<int> keys;

    if(n->children.size() != 0) { // leaf node

        auto temp = getKeysHelper(n->children[0]);
        keys.insert(keys.end(), temp.begin(), temp.end()); // insert left subtree

        for(int i = 1; i < n->children.size(); i++) {
            
            keys.push_back(n->keys[i - 1]); // insert key between children
            
            auto temp = getKeysHelper(n->children[i]);
            keys.insert(keys.end(), temp.begin(), temp.end()); // insert right subtree
        }
    }
    else {
        keys.insert(keys.end(), n->keys.begin(), n->keys.end()); // insert all keys if leaf node
    }

    return keys;
}

#endif