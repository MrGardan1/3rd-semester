#include "stack.h"

Stack::Stack() = default;
Stack::~Stack(){clear();}

void Stack::push(const char& key) {
    Node* newNode = new Node(key);
    newNode -> prev = head;
    newNode -> next = nullptr;
    if (head) {
        head ->next = newNode;
    }
    head = newNode;
    count++;
}

bool Stack::pop(char& out) {
    if (!head) return false;
    out = head->data;
    Node* del = head;
    head = head->prev;
    if (head) head->next = nullptr;
    delete del;
    --count;
    return true;
}

bool Stack::empty() const {
    return count == 0;
}

bool Stack::top(char& out) const {
    if (!head) return false;
    out = head->data;
    return true;
}

int Stack::size() const {
    return count;
}

void Stack::clear() {
    while (head) {
        Node* left = head->prev;
        delete head;
        head = left;
    }
    count = 0;
}