#pragma once

#include <iostream>
using namespace std;

struct Node {
        Node* prev;
        int key;
        int value;
        Node* next;

        Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class DoubleLL {
 private:
    Node* head;
    Node* tail;

 public:
    DoubleLL();
    ~DoubleLL();
    void addToFront(Node* node);
    void removeNode(Node* node);
    Node* removeLast();
    void moveToFront(Node* node);
};

struct HashNode {
    int key;
    Node* listNode;
    HashNode* next;
    HashNode(int k,Node* line) : key(k), listNode(line), next(nullptr) {} 
};

class HashTable {
 private:
    HashNode** table;
    int size;

    int hashFunc(int key) {
        return key % size;
    }


 public:
    HashTable(int s);
    ~HashTable();
    Node* get(int key);
    void put(int key, Node* listNode);
    void remove(int key);
};



class LRUCache {
private:
    int capacity;
    int size;
    DoubleLL list;
    HashTable table;

public:
    LRUCache(int cap);

    int get(int key);
    void set(int key, int value);
};