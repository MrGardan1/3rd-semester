#pragma once

#include <iostream>
#include <fstream>

struct Node {
    int key;
    Node* prev;
    Node* next;
    Node(int k) : key(k), prev(nullptr), next(nullptr) {}
};

class HashTable {
public:
    HashTable(size_t size = 1000);
    ~HashTable();

    void SetAdd(int key);
    void SetDel(int key);
    bool Set_At(int key) const;

    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename) const;
    void clear();

private:
    size_t HashFunc(int key) const;
    size_t count;
    Node** table;
};
