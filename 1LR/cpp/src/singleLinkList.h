#ifndef SINGLE_LINK_LIST_H
#define SINGLE_LINK_LIST_H

#include <iostream>
#include <string>
using namespace std;

struct SLNode {
    string key;
    SLNode* next;
};

struct SingleLL {
    SLNode* head = nullptr;
    int size = 0;
};

void CreateSL(SingleLL& ls, string key);
void OutputSL(SingleLL& ls);
void FPushFront(SingleLL& ls, string key);
void FPushBack(SingleLL& ls, string key);
void FPushAt(SingleLL& ls, int index, string key);
void FPushAfter(SingleLL& ls, string target, string key);
void FPushBefore(SingleLL& ls, string target, string key);
void FGetValue(SingleLL& ls, string key);
string FGetIndex(SingleLL& ls, int index);
void FDelValue(SingleLL& ls, string key);
void FDelAt(SingleLL& ls, int index);
void FDelFront(SingleLL& ls);
void FDelBack(SingleLL& ls);
void FDelAfter(SingleLL& ls, string target);
void FDelBefore(SingleLL& ls, string target);
int FGetSize(SingleLL& ls);
void FDelList(SingleLL& ls);


#endif