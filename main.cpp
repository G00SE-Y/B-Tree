#include <iostream>

#include "BTree.h"

#define TEST_MIN 1
#define TEST_MAX 20

int main(void) {

    std::cout << "hello!" << std::endl;
    BTree t = BTree(3);
    std::cout << "tree made!" << std::endl;
    int i;
    for(i = TEST_MIN; i <= TEST_MAX; i++) {
        std::cout << "Inserting " << i << std::endl;
        t.printNodes();
        t.insert(i);
        t.printNodes();
    }
    std::cout << "-------------------------" << std::endl;
    std::cout << "Inserted " << TEST_MIN << "-" << TEST_MAX << " in order" << std::endl;

    t.printInOrder();
    t.printNodes();

    t.~BTree();

    return 0;
}