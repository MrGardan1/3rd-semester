#include "set.h"

Set::Set(int count)
    : capacity(count > 0 ? count * 2 : 1000), count(0) {
    table = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

Set::~Set() {
    clear();
    delete[] table;
}

size_t Set::hashFunc(int key) const {
    return static_cast<size_t>(key >= 0 ? key : -key) % capacity;
}

Set::Node* Set::findNode(int key) const {
    size_t index = hashFunc(key);
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) return curr;
        curr = curr->next;
    }
    return nullptr;
}

bool Set::contains(int value) const {
    return findNode(value) != nullptr;
}

int Set::getSize() const {
    return count;
}

void Set::insert(int value) {
    if (contains(value)) return;

    size_t index = hashFunc(value);
    Node* newNode = new Node(value);
    newNode->next = table[index];
    if (table[index]) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
    ++count;
}

void Set::erase(int value) {
    Node* node = findNode(value);
    if (!node) return;

    size_t index = hashFunc(value);
    if (node->prev) node->prev->next = node->next;
    else table[index] = node->next;

    if (node->next) node->next->prev = node->prev;

    delete node;
    --count;
}

int Set::size() const {
    return count;
}

void Set::clear() {
    for (int i = 0; i < capacity; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        table[i] = nullptr;
    }
    count = 0;
}

void mainChangeSet(Set &set1, Set &set2) {
    while (true) {
        cout << "\n--- Change set menu ---\n";
        cout << "Select set:\n";
        cout << "1. First set\n";
        cout << "2. Second set\n";
        cout << "3. Back to main menu\n";
        cout << "Choice: ";

        int setChoice;
        if (!(cin >> setChoice)) {
            cout << "Error: choice must be integer\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (setChoice == 3) {
            break;
        }

        Set *current = nullptr;
        const char* setName = nullptr;
        if (setChoice == 1) {
            current = &set1;
            setName = "Set1";
        } else if (setChoice == 2) {
            current = &set2;
            setName = "Set2";
        } else {
            cout << "Please, select 1, 2 or 3" << endl;
            continue;
        }

        bool backToSetSelect = false;
        while (!backToSetSelect) {
            cout << "\n--- Edit " << setName << " ---\n";
            cout << "1. Insert element\n";
            cout << "2. Erase element\n";
            cout << "3. Print set\n";
            cout << "4. Clear set\n";
            cout << "5. Back to set select\n";
            cout << "Choice: ";

            int op;
            if (!(cin >> op)) {
                cout << "Error: choice must be integer\n";
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            switch (op) {
                case 1: {
                    cout << "Enter value to insert: ";
                    int val;
                    if (!(cin >> val)) {
                        cout << "Error: value must be integer\n";
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else {
                        if (current->contains(val)) {
                            cout << "Element already exists in set\n";
                        } else {
                            current->insert(val);
                            cout << "Inserted\n";
                        }
                    }
                    break;
                }
                case 2: {
                    cout << "Enter value to erase: ";
                    int val;
                    if (!(cin >> val)) {
                        cout << "Error: value must be integer\n";
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else {
                        if (!current->contains(val)) {
                            cout << "Element not found in set\n";
                        } else {
                            current->erase(val);
                            cout << "Erased\n";
                        }
                    }
                    break;
                }
                case 3: {
                    cout << setName << " [" << current->size() << "]: ";
                    current->printSet();
                    break;
                }
                case 4: {
                    current->clear();
                    cout << setName << " cleared\n";
                    break;
                }
                case 5: {
                    backToSetSelect = true;
                    break;
                }
                default:
                    cout << "Please, select 1, 2, 3, 4 or 5\n";
                    break;
            }
        }
    }
}

void Set::printSet() const {
    for (int i = 0; i < capacity; ++i) {
        Node* curr = table[i];
        while (curr) {
            std::cout << curr->key << " ";
            curr = curr->next;
        }
    }
    std::cout << std::endl;
}

Set Set::unionSets(const Set& set1, const Set& set2) {
    Set result(set1.size() + set2.size());
    for (int i = 0; i < set1.capacity; ++i) {
        Node* curr = set1.table[i];
        while (curr) {
            result.insert(curr->key);
            curr = curr->next;
        }
    }
    for (int i = 0; i < set2.capacity; ++i) {
        Node* curr = set2.table[i];
        while (curr) {
            result.insert(curr->key);
            curr = curr->next;
        }
    }
    return result;
}

Set Set::intersectSets(const Set& set1, const Set& set2) {
    Set result(set1.size() < set2.size() ? set1.size() : set2.size());
    for (int i = 0; i < set1.capacity; ++i) {
        Node* curr = set1.table[i];
        while (curr) {
            if (set2.contains(curr->key)) result.insert(curr->key);
            curr = curr->next;
        }
    }
    return result;
}

Set Set::differSets(const Set& set1, const Set& set2) {
    Set result(set1.size());
    for (int i = 0; i < set1.capacity; ++i) {
        Node* curr = set1.table[i];
        while (curr) {
            if (!set2.contains(curr->key)) result.insert(curr->key);
            curr = curr->next;
        }
    }
    return result;
}


