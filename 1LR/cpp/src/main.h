#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#include "singleLinkList.h"
#include "array.h"
#include "stack.h"
#include "queue.h"
#include "doubleLinkList.h"
#include "tree.h"

void mainCommand(const string& command);

void saveArrayToFile(Array& arr, const string& filename);
void loadArrayFromFile(Array& arr, const string& filename);

void saveListToFile(SingleLL& list, const string& filename);
void loadListFromFile(SingleLL* list, const string& filename);

void saveStackToFile(Stack* stack, const string& filename);
void loadStackFromFile(Stack*& stack, const string& filename);

void saveQueueToFile(Queue* queue, const string& filename);
void loadQueueFromFile(Queue*& queue, const string& filename);

void saveDoubleListToFile(DoubleList* list, const string& filename);
void loadDoubleListFromFile(DoubleList*& list, const string& filename);

void saveTreeToFile(Tree* tree, const string& filename);
void loadTreeFromFile(Tree*& tree, const string& filename);


void printMain();
void printAll();
void printM();
void printF();
void printL();
void printS();
void printQ();
void printT();


#endif