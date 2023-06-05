
typedef struct Node {
    
    int key; // data
    int nKeys; //number of children
    struct Node* keys; // pointers to children
    bool isLeaf;
}node;

class BTree {

    private:

        node* initNode(int v);


    public:

        BTree(int branchFactor);
        ~BTree();
        void insert(int v);
        bool search(int v);
        

};