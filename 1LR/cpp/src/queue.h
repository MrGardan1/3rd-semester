#ifndef DEQUEUE_H
#define DEQUEUE_H

#include <iostream>
using namespace std;

struct QNode {
    string key;
    QNode* past;
    QNode* next;
};

struct Queue {
    QNode* front;
    QNode* back;
    int size;
};

void CreateQ(Queue& q);
void OutputQ(Queue& q);
void QPush(Queue& q, string key);
void QPop(Queue& q);
int QGetSize(Queue& q);
bool isEmpty(Queue*  queue);
string QGetIndex(Queue& q, int index);
void QDelAll(Queue* q);

#endif 