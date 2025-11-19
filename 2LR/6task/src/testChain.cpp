#include "testChain.h"

chainHashMap::chainHashMap(size_t s) : size(s), count(0) {
    table = new Node*[size];
    for (size_t i = 0; i < size; i++) {
        table[i] = nullptr;
    }
}

chainHashMap::~chainHashMap() {
    clear();
    delete[] table;
}

void chainHashMap::clear() {
    for (size_t i = 0; i < size; i++) {
        Node* curr = table[i];
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        table[i] = nullptr;
    }
}

void chainHashMap::insert(int key, int value) {
    size_t index = hashFunc(key);
    Node* curr = table[index];

    while (curr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }

    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    if (table[index]) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
}

void chainHashMap::remove(int key) {
    size_t index = hashFunc(key);
    Node* curr = table[index];

    while (curr && curr->key != key) {
        curr = curr->next;
    }
    if (!curr) return;

    if (curr->prev) {
        curr->prev->next = curr->next;
    } else {
        table[index] = curr->next;
    }

    if (curr->next) {
        curr->next->prev = curr->prev;
    }

    delete curr;
}

//ВОЗВРАЩАЕТ ЗНАЧЕНИЕ А BOOL
int chainHashMap::search(int key) const {
    size_t index = hashFunc(key);
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    return -1;
}
