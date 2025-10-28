#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
using namespace std;

struct Stack{
    string* data;
    int size;
    int capacity;
};

void CreateS(Stack& st);
void OutputS(Stack& st);
void SPush(Stack* st, string key);
string SPop(Stack* st);
void SDelAll(Stack* st);
string SPeek(Stack* st);
int SGetSize(Stack* st);
bool isEmpty(Stack* st);

#endif