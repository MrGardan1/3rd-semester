#pragma once

#include <iostream>
#include <string>
using namespace std;



class Stack {
 public:
    void push(const char& key);
    bool pop(char& out);
    bool empty() const;
    bool top(char& out) const;
    int size() const;
    void clear();
    Stack();
    ~Stack();
 private:
    struct Node {
        Node* prev;
        char data;
        Node* next;
        explicit Node(char d) : prev(nullptr), data(d), next(nullptr) {}
    };

    Node* head = nullptr;
    int count = 0;
    
};

