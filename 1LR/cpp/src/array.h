#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <string>
using namespace std;

struct Array {
    string* data;
    int size;
    int capacity;
};

void MCreate(Array& arr);
void MDelIndex(Array& arr, int index);
void OutputM(Array& arr);
void OutputM(Array& arr, int index);
void MInsert(Array& arr, int index, string key);
void MReplace(Array& arr, int index, string key);
void MPushBack(Array& arr, string key);
int MGetSize(Array& arr);
string MGetValue(Array& arr, int index);

#endif