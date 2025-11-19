#pragma once

#include <iostream>
using namespace std;

class Tree {
 public:
    
    Tree();
    ~Tree();

    void insert(int key);
    bool isAVL();

 private:
    struct Node {
        
        int key;
        Node* left;
        Node* right;
        int height;

        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };
    Node* root;

    Node* insertRec(Node* node, int key);
    bool isAVLRec (Node* node);
    void deleteTree(Node* tree);
    void clearTree(Node* node);
};