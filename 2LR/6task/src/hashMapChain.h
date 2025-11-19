#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <climits>

using namespace std;

struct Node {
    int key;
    Node* prev;
    Node* next;
    Node(int k) : key(k), prev(nullptr), next(nullptr) {}
};

class HashMapChain {
 private:
    size_t hashFunc(int key) const;
    size_t count;
    Node** table;
    void clear();

 public:
    HashMapChain(size_t s);
    ~HashMapChain();
    void insert(int key);
    void remove(int key);
    void getChainLengths(int &minLen, int &maxLen);

};