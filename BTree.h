#ifndef BTREE_H
#define BTREE_H

#include <vector>

typedef struct Node {
    
    bool isLeaf; // true -> leaf node ; false -> internal node
    std::vector<int> keys;
    std::vector<struct Node*> children; // pointers to children nodes
    
}node;

class BTree {

    private:

        int size; // branch factor
        int keyCount; // number of stored keys
        int nodeCount; // number of nodes
        int height; // height of the tree

        node* root; // root node of the tree

        node* initNode(); // create default node
        void cleanup(node* r); // free up all tree nodes
        void splitChild(node* x, int i, node* y); // turn an overfull node into a subtree
        void insertNonfull(node* x, int k); // insert normally
        bool contains(node* x, int k); // recursively find key with binary search

        void printHelper(node* n); // debug method
        void printNodesHelper(node* n, int depth); // debug method
        std::vector<int> getKeysHelper(node* n); // debug method

    public:

        BTree(int branchFactor);
        ~BTree();
        void insert(int k);
        bool contains(int k);

        void printInOrder();
        void printNodes();

        int getsize();
        int getKeyCount();
        int getnodeCount();
        int getHeight();
        std::vector<int> getKeys();       
};

#endif