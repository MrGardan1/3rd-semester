#include "tree.h"

Tree::Tree() {
    root = nullptr;
}

void Tree::clearTree(Node* node) {
    if (!node) return;
    clearTree(node ->left);
    clearTree(node->right);
    delete node;
}

void Tree::deleteTree(Node* tree) {
    if (tree) {
        clearTree(root);
    }
}

Tree::~Tree() {
    deleteTree(root);
}

Tree::Node* Tree::insertRec(Node* node, int key) {
    if (!node) return new Node(key);

    if (key < node->key) {
        node -> left = insertRec(node->left, key);
    } else {
        node ->right = insertRec(node ->right, key);
    }
    return node;
}

void Tree::insert(int key) {
    root = insertRec(root, key);
}

bool Tree::isAVLRec(Node* node) {
    if (!node) return 0;
        int lh = isAVLRec(node->left);
        if (lh == -1) return -1;
        int rh = isAVLRec(node->right);
        if (rh == -1) return -1;
        if (abs(lh - rh) > 1) return -1;
        return max(lh, rh) + 1;
}

bool Tree::isAVL() {
        return isAVLRec(root) != 0;
}