#include <iostream>
#include <cmath>
#include <string>
#include <random>
#include <algorithm>

#include "BTree.h"

// This script takes a while

#define TEST_MIN_KEYS 1000000
#define TEST_MAX_KEYS 1000000 // 1,000,000
#define TEST_MIN_DEGREE 2
#define TEST_MAX_DEGREE 100 // 1,000

#define SEED 0
#define N_RAND_TESTS 100

// prototypes
bool check_height(int t, int h, int n);
bool check_order(std::vector<int> k, std::vector<int> v);
bool ordered_test(int low, int high, int deg, std::string name);
bool rand_test(int low, int high, int deg, int num, std::string name);


int main(void) {

    srand(SEED);

    int fails = 0;
    int tests = 0;
    std::cout << std::endl;

    tests++;
    if(!ordered_test(1, 10, 2, "basic_test(1)")) {
        fails++;
    }

    tests++;
    if(!ordered_test(1, 20, 2, "basic_test(2)")) {
        fails++;
    }

    tests++;
    if(!ordered_test(1, 10, 3, "basic_test(3)")) {
        fails++;
    }

    tests++;
    if(!ordered_test(1, 20, 3, "basic_test(4)")) {
        fails++;
    }

    tests++;
    if(!ordered_test(1, 100, 3, "basic_test(5)")) {
        fails++;
    }

    tests++;
    if(!ordered_test(1, 10000, 10, "not_so_basic_test")) {
        fails++;
    }

    int n, d;
    std::string s = "Random_Test(", ts;

    for(int i = 0; i < N_RAND_TESTS; i++) {

        n = (rand() + TEST_MIN_KEYS) % TEST_MAX_KEYS; // number of keys
        d = (rand() + TEST_MIN_DEGREE) % TEST_MAX_DEGREE; // degree of btree

        ts = s + std::to_string(i + 1) + ")";
        
        tests++;
        if(!rand_test(1, TEST_MAX_KEYS, TEST_MAX_DEGREE, TEST_MAX_KEYS, ts)) fails++;
    }


    std::cout << "------------------------" << std::endl;
    if(fails == 0) {
        std::cout << "All cases passed!" << std::endl;
    }
    else {
        std::cout << "Failed " << fails  << "/" << tests << " cases...\n" << std::endl;
    }
    
    return 0;
}


bool check_height(int t, int h, int n) {

    int h_min = ceil(log(n + 1) / log(2 * t));
    int h_max = floor(log((n + 1) / 2) / log(t)) + 1;

    // std::cout << "Expected " << h_min << " <= H <= " << h_max << std::endl;

    if(h_min <= h && h <= h_max) return true;
    else return false;
}


bool check_order(std::vector<int> k, std::vector<int> v) {
    
    if(k.size() != v.size()) return false; // sizes don't match

    for(int i = 0; i < k.size(); i++) {
        
        if(k[i] != v[i]) return false; // order doesn't match
    }

    return true;
}


bool ordered_test(int low, int high, int deg, std::string name) {
    
    std::cout<< "Test: " + name << "\n" << std:: endl; 
    std::cout<< "\tParams: {range: [" << low << " .. " << high << "], deg: " << deg << "}" << std::endl;

    BTree t = BTree(deg);
    
    std::vector<int> v;
    for(int i = low; i <= high; i++) { // insert in order from min to max
        v.push_back(i);
        t.insert(i);
    }

    int k = t.getKeyCount();
    int n = t.getnodeCount();
    int h = t.getHeight();

    std::cout << "\t{keys: " << k << ", " 
              << "nodes: " << n << ", " 
              << "height: " << h << "}" << std::endl;

    std::string err = "";
    bool res = true;

    if(!check_height(deg, h, k)) {
        err += "\tHeight does not match!\n";
        res = false;
    }

    if(!check_order(t.getKeys(), v)) {
        err += "\tValues are not in order!\n";
        res = false;
    }

    std::cout << err << std::endl;

    if(res) {
        std::cout << "\tPassed!\n" << std::endl;
    }

    // t.printInOrder();
    // t.printNodes();

    return res;
}


bool rand_test(int low, int high, int deg, int num, std::string name) {

    std::cout<< "Test: " + name << "\n" << std:: endl; 
    std::cout<< "\tParams: {range: [" << low << " .. " << high << "], deg: " << deg << ", keys: " << num << "}" << std::endl;

    BTree t = BTree(deg);
    
    std::vector<int> v;
    int r;
    for(int i = 0; i <= num; i++) { // insert in order from min to max

        r = (rand() + low) % high;

        // std::cout << "(" << i << ") " << r << " " << std::endl;

        v.push_back(r);
        t.insert(r);
    }

    // std::cout << "helloo" << std::endl;

    std::sort(v.begin(), v.end());

    int k = t.getKeyCount();
    int n = t.getnodeCount();
    int h = t.getHeight();

    std::cout << "\t{keys: " << k << ", " 
              << "nodes: " << n << ", " 
              << "height: " << h << "}" << std::endl;

    std::string err = "";
    bool res = true;

    if(!check_height(deg, h, k)) {
        err += "\tHeight does not match!\n";
        res = false;
    }

    if(!check_order(t.getKeys(), v)) {
        err += "\tValues are not in order!\n";
        res = false;
    }

    std::cout << err << std::endl;

    if(res) {
        std::cout << "\tPassed!\n" << std::endl;
    }

    return res;
}