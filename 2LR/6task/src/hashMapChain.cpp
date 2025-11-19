#include "hashMapChain.h"

HashMapChain::HashMapChain(size_t size) : count(size) {
    table = new Node*[count];
    for (size_t i = 0; i < count; ++i) {
        table[i] = nullptr;
    }
}

HashMapChain::~HashMapChain() {
    clear();
    delete[] table;
}

size_t HashMapChain::hashFunc(int key) const {
    return key % count;
}

void HashMapChain::clear() {
    for (size_t i = 0; i < count; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        table[i] = nullptr;
    }
}

void HashMapChain::insert(int key) {
    size_t index = hashFunc(key);
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) {
            return;
        }
        curr = curr->next;
    }
    Node* newNode = new Node(key);
    newNode->next = table[index];
    if (table[index]) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
}

void HashMapChain::remove(int key) {
    size_t index = hashFunc(key);
    Node* curr = table[index];

    while (curr && curr->key != key) {
        curr = curr->next;
    }
    if (!curr) {
        cout << "Element not found\n";
        return;
    }
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

void HashMapChain::getChainLengths(int &minLen, int &maxLen) {
    minLen = INT_MAX;
    maxLen = 0;

    for (size_t i = 0; i < count; ++i) {
        int length = 0;
        Node* curr = table[i];
        while (curr) {
            ++length;
            curr = curr->next;
        }

        if (length < minLen) {
            minLen = length;
        }
        if (length > maxLen) {
            maxLen = length;
        }
    }
}
