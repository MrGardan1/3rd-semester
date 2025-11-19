#pragma once
#include <iostream>
#include <limits>
using namespace std;

class Set {
public:
    Set(int count);
    ~Set();

    void insert(int value);
    void erase(int value);
    bool contains(int value) const;
    int size() const;
    void printSet() const;
    void clear();
    int getSize() const;

    static Set unionSets(const Set& set1, const Set& set2);
    static Set intersectSets(const Set& set1, const Set& set2);
    static Set differSets(const Set& set1, const Set& set2);

private:
    struct Node {
        int key;
        Node* prev;
        Node* next;
        Node(int k) : key(k), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    int count;
    Node** table;

    size_t hashFunc(int key) const;
    Node* findNode(int key) const;
};

void mainChangeSet(Set& set1, Set& set2);
