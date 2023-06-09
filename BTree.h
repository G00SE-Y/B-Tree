#include <vector>

typedef struct Node {
    
    bool isLeaf; // is this node a leaf or not
    int nKeys; // number of keys in node
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

        node* initNode();
        void cleanup(node* r);
        void splitChild(node* x, int i, node* y);
        void insertNonfull(node* x, int k);
        void printHelper(node* n);
        void printNodesHelper(node* n, int depth);

    public:


        BTree(int branchFactor);
        ~BTree();
        void insert(int k);
        bool search(node* x, int k);
        void printInOrder();
        void printNodes();
        
};