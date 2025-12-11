#pragma once
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

class AvlTree {
   public:
    void TINSERT(int key) { root = insert(std::move(root), key); }

    void TDEL(int key) { root = remove(std::move(root), key); }

    bool ISMEMBER(int key) const { return contains(root, key); }

    std::string TGET(int key) const { return contains(root, key) ? std::to_string(key) : ""; }

    std::string PRINT_PREORDER() const {
        std::vector<int> res;
        preorder(root, res);
        return vecToString(res);
    }

    std::string PRINT_INORDER() const {
        std::vector<int> res;
        inorder(root, res);
        return vecToString(res);
    }

    std::string PRINT_POSTORDER() const {
        std::vector<int> res;
        postorder(root, res);
        return vecToString(res);
    }

    std::string PRINT_BFS() const {
        std::vector<int> res;
        bfs(root, res);
        return vecToString(res);
    }

   private:
    struct Node {
        int key;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

    int height(const std::unique_ptr<Node>& n) const { return n ? n->height : 0; }

    int getBalance(const std::unique_ptr<Node>& n) const {
        return n ? height(n->right) - height(n->left) : 0;
    }

    void updateHeight(std::unique_ptr<Node>& n) {
        if (n) {
            int hl = height(n->left);
            int hr = height(n->right);
            n->height = std::max(hl, hr) + 1;
        }
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {
        std::unique_ptr<Node> y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        updateHeight(y->left);
        updateHeight(y);
        return y;
    }

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {
        std::unique_ptr<Node> x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        updateHeight(x->right);
        updateHeight(x);
        return x;
    }

    std::unique_ptr<Node> balance(std::unique_ptr<Node> node) {
        updateHeight(node);
        int balanceFactor = getBalance(node);
        if (balanceFactor > 1) {
            if (getBalance(node->right) < 0) {
                node->right = rotateRight(std::move(node->right));
            }
            return rotateLeft(std::move(node));
        }
        if (balanceFactor < -1) {
            if (getBalance(node->left) > 0) {
                node->left = rotateLeft(std::move(node->left));
            }
            return rotateRight(std::move(node));
        }
        return node;
    }

    std::unique_ptr<Node> insert(std::unique_ptr<Node> node, int key) {
        if (!node) return std::make_unique<Node>(key);
        if (key < node->key)
            node->left = insert(std::move(node->left), key);
        else if (key > node->key)
            node->right = insert(std::move(node->right), key);
        else
            return node;
        return balance(std::move(node));
    }

    std::unique_ptr<Node> remove(std::unique_ptr<Node> node, int key) {
        if (!node) return nullptr;

        if (key < node->key)
            node->left = remove(std::move(node->left), key);
        else if (key > node->key)
            node->right = remove(std::move(node->right), key);
        else {
            // Узел с 0 или 1 ребенком
            if (!node->left) return std::move(node->right);
            if (!node->right) return std::move(node->left);

            // Узел с 2 детьми: копируем минимальный из правого поддерева
            Node* minNode = node->right.get();
            while (minNode->left) {
                minNode = minNode->left.get();
            }
            node->key = minNode->key;
            node->right = remove(std::move(node->right), minNode->key);
        }
        return balance(std::move(node));
    }

    bool contains(const std::unique_ptr<Node>& node, int key) const {
        if (!node) return false;
        if (key == node->key)
            return true;
        else if (key < node->key)
            return contains(node->left, key);
        else
            return contains(node->right, key);
    }

    void preorder(const std::unique_ptr<Node>& node, std::vector<int>& result) const {
        if (!node) return;
        result.push_back(node->key);
        preorder(node->left, result);
        preorder(node->right, result);
    }

    void inorder(const std::unique_ptr<Node>& node, std::vector<int>& result) const {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->key);
        inorder(node->right, result);
    }

    void postorder(const std::unique_ptr<Node>& node, std::vector<int>& result) const {
        if (!node) return;
        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->key);
    }

    void bfs(const std::unique_ptr<Node>& node, std::vector<int>& result) const {
        if (!node) return;
        std::queue<const Node*> q;
        q.push(node.get());
        while (!q.empty()) {
            const Node* n = q.front();
            q.pop();
            result.push_back(n->key);
            if (n->left) q.push(n->left.get());
            if (n->right) q.push(n->right.get());
        }
    }

    std::string vecToString(const std::vector<int>& vec) const {
        if (vec.empty()) return "";
        std::ostringstream oss;
        for (size_t i = 0; i < vec.size(); ++i) {
            oss << vec[i];
            if (i + 1 < vec.size()) oss << " ";
        }
        return oss.str();
    }
};
