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

        node* root;
        node* initNode();
        void cleanup(node* r);
        void splitChild(node* x, int i, node* y);
        void insertNonfull(node* x, int k);


    public:

        BTree(int branchFactor);
        ~BTree();
        void insert(int k);
        bool search(node* x, int k);
        

};