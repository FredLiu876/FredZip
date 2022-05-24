#include "IndexList.h"
#include <iostream>
using namespace std;

IndexList::IndexNode::IndexNode() {
    next = nullptr;
}

IndexList::IndexNode::~IndexNode() {
    delete next;
}

IndexList::IndexList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

IndexList::~IndexList() {
    delete head;
}

void IndexList::push(unsigned int newIndex) {
    IndexList::IndexNode *newNode = new IndexList::IndexNode;
    newNode->index = newIndex;
    if (head == nullptr) {
        head = newNode;
    } else {
        tail->next = newNode;
    }
    tail = newNode;

    size += 1;
}

void IndexList::print() {
    // for testing, otherwise unused method
    IndexList::IndexNode *selected = head;
    for (unsigned int i = 0; i < size; i++) {
        if (i != 0) {
            selected = selected->next;
        }
        cout << selected->index << " ";
    }
    cout << endl;
}

unsigned int IndexList::getSize() const {
    unsigned int newSize = size;
    return newSize;
}