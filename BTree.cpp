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

    node* r = root;

    if(root->keys.size() == 2 * size - 1) { // root is full
        // std::cout << "Root Full. ";
        
        node* s = initNode();
        root = s;

        s->isLeaf = false;
        s->children.push_back(r);

        splitChild(s, 0, r);
        insertNonfull(s, k);

        height++;
    }
    else { // root is not full
        insertNonfull(r, k);
    }
    keyCount++;
}


void BTree::insertNonfull(node* x, int k) {

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

        if(x->children[i]->keys.size() == 2 * size - 1) {

            splitChild(x, i, x->children[i]);

            if(k > x->keys[i]) i++; // the key at i might have changed after the split
        }

        insertNonfull(x->children[i], k);
    }
}


void BTree::splitChild(node* x, int i, node* y) {

    node* z = initNode();

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
}


bool BTree::contains(int k) {
    return contains(root, k);
}


bool BTree::contains(node* x, int k) {

    int l = 0;
    int r = x->keys.size() - 1;
    int m;
    while(l <= r && l >= 0 && r < x->keys.size()) { // binary search
        m = l + (r - l) / 2;
        
        // std::cout << "from " << l << " to " << r << ":  keys[" << m << "] = " << x->keys[m] << std::endl;

        if(x->keys[m] == k) return true; // found

        if(x->keys[m] > k) {
            r = m - 1;
        }
        else {
            l = m + 1;
        }
    }

    // if leaf, don't traverse anymore
    if(x->isLeaf) return false;

    // internal node, find child to traverse to
    if(k < x->keys[m]) {
        // std::cout << "Traversing to child at " << m << std::endl;
        return contains(x->children[m], k); // left child
    }
    else  {
        // std::cout << "Traversing to child at " << m + 1 << std::endl;
        return contains(x->children[m + 1], k); // right child
    }
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