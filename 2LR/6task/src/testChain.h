#pragma once
#include <iostream>
#include <cstdlib>
#include <climits>
using namespace std;

struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class chainHashMap {
private:
    size_t count;
    size_t size;
    Node** table;

    size_t hashFunc(int key) const {
        return static_cast<size_t>(key >= 0 ? key : -key) % size;
    }

    void clear();

public:
    chainHashMap(size_t s);
    ~chainHashMap();

    void insert(int key, int value);
    void remove(int key);
    int search(int key) const;
};
