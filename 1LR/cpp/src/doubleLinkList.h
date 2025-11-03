#ifndef DOUBLELINKLIST_H
#define DOUBLELINKLIST_H

#include <iostream>
using namespace std;

struct DLNode {
    string key;
    DLNode* past;
    DLNode* next;
};

struct DoubleList {
    DLNode* head;
    DLNode* tail;
    int size;
};

void CreateDL(DoubleList& ls);
void OutputDL(DoubleList& ls);
void RevOutputDL(DoubleList& ls);

void DLPushFront(DoubleList& ls, string key);
void DLPushBack(DoubleList& ls, string key);
void DLPushAt(DoubleList& ls, int index, string key);

void DLFind(DoubleList& ls, string key);
void DLPushAft(DoubleList& ls,string target, string key);
void DLPushBef(DoubleList& ls, string target, string key);
void DLDelValue(DoubleList& ls, string key);
void DLDelAt(DoubleList& ls, int index);
void DLDelFront(DoubleList& ls);
void DLDelBack(DoubleList& ls);
void DLDelAfter(DoubleList& ls, string target);
void DLDelBefore(DoubleList& ls, string target);
void DLDelAll(DoubleList& ls);
int DLGetSize(DoubleList& ls);
string DLGetAt(DoubleList& ls, int index);
string DLGetFront(DoubleList& ls);
string DLGetBack(DoubleList& ls);

#endif