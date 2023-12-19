#include <iostream>
#include <vector>

#include "BTree.h"


// getters
int BTree::getsize() { return size; }
int BTree::getKeyCount() { return keyCount; }
int BTree::getnodeCount() { return nodeCount; }
int BTree::getHeight() { return height; }


node* BTree::initNode() {
    node* n = new node;
    nodeCount++;
    n->isLeaf = true;
    return n;
}


BTree::BTree(int branchFactor) {
    size = branchFactor; // full nodes have (2 * size - 1) KEYS, and (2 * size) CHILDREN
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
    // std::cout << "Inserting " << k << ". ";
    node* r = root;

    if(root->keys.size() == 2 * size - 1) { // root is full
        // std::cout << "Root Full. ";
        
        node* s = initNode();
        root = s;

        s->isLeaf = false;
        s->children.push_back(r);

        // std::cout << "Splitting Child... ";
        splitChild(s, 0, r);
        // std::cout << "Child Split." << std::endl;
        insertNonfull(s, k);

        height++;
    }
    else { // root is not full
        insertNonfull(r, k);
    }
    keyCount++;

    // printInOrder();
}


void BTree::insertNonfull(node* x, int k) {
    
    // std::cout << "Trying to insert " << k << " into [ ";
    // for(auto i: x->keys) std::cout << i << " ";
    // std::cout<< "]" << std::endl;

    auto it = x->keys.begin();
    auto begin = x->keys.begin();
    auto end = x->keys.end();

    // find position to insert at
    while(it != end && k > *it) it++;

    if(x->isLeaf) { // insert into leaf
        x->keys.insert(it, k);
    }
    else {
    
        int i = std::distance(begin, it);
        // std::cout << "Moving into child at " << i << std::endl;

        if(x->children[i]->keys.size() == 2 * size - 1) {

            splitChild(x, i, x->children[i]);

            if(k > x->keys[i]) i++; // the key at i might have changed after the split
        }

        insertNonfull(x->children[i], k);
    }
}


void BTree::splitChild(node* x, int i, node* y) {

    node* z = initNode();

    // std::cout << "Before split: i:" << i << " [ ";
    // for(auto i: y->keys) std::cout << i << " ";
    // std::cout << "]" << std::endl;

    z->isLeaf = y->isLeaf;
    z->keys.insert(z->keys.begin(), y->keys.begin() + size, y->keys.end()); // copy right half of node y's keys onto node z
    y->keys.erase(y->keys.begin() + size, y->keys.end()); // delete copies from y

    if(!y->isLeaf) {
        z->children.insert(z->children.begin(), y->children.begin() + size, y->children.end()); // copy right half of node y's children onto node z
        y->children.erase(y->children.begin() + size, y->children.end()); // delete copies from y
    }

    x->children.insert(x->children.begin() + i + 1, z);
    x->keys.insert(x->keys.begin() + i, y->keys.back());
    y->keys.pop_back();

    // std::cout << "Sanity: [ ";
    // for(auto i: y->keys) std::cout << i << " ";
    // std::cout << "]  " << x->keys[i] << "  [ ";
    // for(auto i: z->keys) std::cout << i << " ";
    // std::cout << "]" << std::endl;
}


bool BTree::search(node* x, int k) {

    return false;
}


void BTree::printInOrder() {

    std::cout << "BTREE:" << std::endl;
    if(root->keys.size() == 0) {
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
    
    int keys = n->keys.size();

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
    if(root->keys.size() == 0) {
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

    for(int i = 0; i < n->keys.size() - 1; i++) {
        std::cout << n->keys[i] << ", ";
    }
    std::cout << n->keys.back() << "]" << std::endl;

    for(auto c: n->children) {
        printNodesHelper(c, depth + 1);
    }
}


std::vector<int> BTree::getKeys() {

    std::vector<int> keys;

    if(root->keys.size() != 0) {

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