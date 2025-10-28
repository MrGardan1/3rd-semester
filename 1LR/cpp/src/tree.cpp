#include "tree.h"

void initTree(Tree* tree) {
    tree->root = nullptr;
    tree->size = 0;
}

void deleteTree(Tree* tree) {
    clearTree(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}

int getHeight(TreeNode* node) {
    return node ? node->height : 0;
}

int getBalance(TreeNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

void updateHeight(TreeNode* node) {
    if (node) {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
}

//правый поворот
TreeNode* rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;
}

//левый поворот
TreeNode* rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    updateHeight(x);
    updateHeight(y);
    
    return y;
}

//вставка узла рекурсивно
TreeNode* insertNode(TreeNode* node, int key, int& size) {
    if (!node) {
        size++;
        TreeNode* newNode = new TreeNode;
        newNode->key = key;
        newNode->height = 1;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    
    if (key < node->key) {
        node->left = insertNode(node->left, key, size);
    } else if (key > node->key) {
        node->right = insertNode(node->right, key, size);
    } else {
        //если есть уже такой узел
        return node;
    }
    
    updateHeight(node);
    
    int balance = getBalance(node);
    
    //балансировка дерева
    //левый-левый поворот
    if (balance > 1 && key < node->left->key) {
        return rotateRight(node);
    }
    
    //правый-правый
    if (balance < -1 && key > node->right->key) {
        return rotateLeft(node);
    }
    
    //левый-правый
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    //правый-левый
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void insert(Tree* tree, int key) {
    tree->root = insertNode(tree->root, key, tree->size);
}

//удаление узла рекурсивно
TreeNode* removeNode(TreeNode* node, int key, int& size) {
    if (!node) return nullptr;
    
    if (key < node->key) {
        node->left = removeNode(node->left, key, size);
    } else if (key > node->key) {
        node->right = removeNode(node->right, key, size);
    } else {
        //если это данный узел для удаления
        if (!node->left || !node->right) {
            TreeNode* temp = node->left ? node->left : node->right;
            
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            
            delete temp;
            size--;
        } else {
            //если два потомка - находим минимальный в правом поддереве
            TreeNode* successor = node->right;
            while (successor->left) {
                successor = successor->left;
            }
            
            //копируем ключ преемника
            node->key = successor->key;
            
            //удаляем преемника рекурсивно
            node->right = removeNode(node->right, successor->key, size);
        }
    }
    
    if (!node) return nullptr;
    
    updateHeight(node);
    
    int balance = getBalance(node);
    
    //балансировка
    //левый-левый
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    
    //левый-правый
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    //правый-правый
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    
    //Право-левый
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

bool remove(Tree* tree, int key) {
    int oldSize = tree->size;
    tree->root = removeNode(tree->root, key, tree->size);
    return tree->size < oldSize;
}

TreeNode* searchNode(TreeNode* node, int key) {
    if (!node || node->key == key) {
        return node;
    }
    if (key < node->key) {
        return searchNode(node->left, key);
    } else {
        return searchNode(node->right, key);
    }
}

bool search(Tree* tree, int key) {
    return searchNode(tree->root, key) != nullptr;
}

void clearTree(TreeNode* node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

// прямой обход (Preorder: корень - лево - право) - DFS
void printPreOrder(TreeNode* node) {
    if (node) {
        cout << node->key << " ";
        printPreOrder(node->left);
        printPreOrder(node->right);
    }
}

void printPreOrder(Tree* tree) {
    cout << "Прямой обход (Preorder): ";
    printPreOrder(tree->root);
    cout << endl;
}

// обратный обход (Postorder: лев - прав - корень) - DFS
void printPostOrder(TreeNode* node) {
    if (node) {
        printPostOrder(node->left);
        printPostOrder(node->right);
        cout << node->key << " ";
    }
}

void printPostOrder(Tree* tree) {
    cout << "Обратный обход (Postorder): ";
    printPostOrder(tree->root);
    cout << endl;
}

// обход в ширину (level-order: по слоям,  ну по уровням) - BFS
void printLevelOrder(TreeNode* node) {
    if (!node) return;
    
    queue<TreeNode*> q;
    q.push(node);
    
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        
        cout << current->key << " ";
        
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}

void printLevelOrder(Tree* tree) {
    cout << "Обход в ширину (Level-order): ";
    printLevelOrder(tree->root);
    cout << endl;
}

//центрированный обход (Inorder: лев - корень - прав) - DFS
void printInOrder(TreeNode* node) {
    if (node) {
        printInOrder(node->left);
        cout << node->key << " ";
        printInOrder(node->right);
    }
}

void printInOrder(Tree* tree) {
    cout << "Центрированный обход (Inorder): ";
    printInOrder(tree->root);
    cout << endl;
}

void saveNodeToFile(TreeNode* node, ofstream& file) {
    if (!node) {
        file << "NULL" << endl;
        return;
    }
    
    file << node->key << " " << node->height << endl;
    
    saveNodeToFile(node->left, file);
    saveNodeToFile(node->right, file);
}

TreeNode* loadNodeFromFile(ifstream& file) {
    string line;
    getline(file, line);
    
    if (line == "NULL") {
        return nullptr;
    }

    istringstream iss(line);
    int key, height;
    iss >> key >> height;
    
    TreeNode* node = new TreeNode;
    node->key = key;
    node->height = height;
    
    node->left = loadNodeFromFile(file);
    node->right = loadNodeFromFile(file);
    
    return node;
}




