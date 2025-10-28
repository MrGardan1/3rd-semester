#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

struct TreeNode {
    int key;
    int height;
    TreeNode* left;
    TreeNode* right;
};

struct Tree {
    TreeNode* root;
    int size;
};

void initTree(Tree* tree);
void deleteTree(Tree* tree);

void insert(Tree* tree, int key);
bool remove(Tree* tree, int key);
bool search(Tree* tree, int key);
void printPreOrder(Tree* tree);
void printPostOrder(Tree* tree);
void printLevelOrder(Tree* tree);
void printInOrder(Tree* tree);

void saveTreeToFile(Tree* tree, const string& filename);
void loadTreeFromFile(Tree*& tree, const string& filename);

int getHeight(TreeNode* node);
int getBalance(TreeNode* node);
TreeNode* rotateRight(TreeNode* y);
TreeNode* rotateLeft(TreeNode* x);
TreeNode* insertNode(TreeNode* node, int key, int& size);
TreeNode* removeNode(TreeNode* node, int key, int& size);
TreeNode* searchNode(TreeNode* node, int key);
void saveNodeToFile(TreeNode* node, ofstream& file);
TreeNode* loadNodeFromFile(ifstream& file);
void clearTree(TreeNode* node);
void printPreOrder(TreeNode* node);
void printPostOrder(TreeNode* node);
void printLevelOrder(TreeNode* node);
void printInOrder(TreeNode* node);

#endif