#include "SubtreeStack.h"
#include "Node.h"

using namespace std;

SubtreeStack::Subtree::Subtree() {
    data = nullptr;
    next = nullptr;
}

SubtreeStack::Subtree::~Subtree() {
    delete next;
}

SubtreeStack::SubtreeStack() {
    size = 0;
    head = nullptr;
}

SubtreeStack::~SubtreeStack() {
    delete head;
}

unsigned int SubtreeStack::getSize() {
    unsigned int returnSize = size;
    return returnSize;
}

void SubtreeStack::push(Node *node) {

    Subtree *newSubtree = new Subtree;
    newSubtree->data = node;
    if (head != nullptr) {
        newSubtree->next = head;
    }
    head = newSubtree;
    size += 1;

}

/* Removes two of the first subtrees and combines them into larger subtree
then pushes larger subtree back into stack */
void SubtreeStack::pop() {

    Node *node1 = head->next->data;
    Node *node2 = head->data;
    Node *combined = new Node;
    combined->set(node1->characters+node2->characters, 0, node1, node2);
    size -= 2;
    if (size == 0) {
        head = nullptr;
    } else {
        head = head->next->next;
    }
    push(combined);

}

void SubtreeStack::print() {
    if (head != nullptr) {
        print(head);
    }
    
}

void SubtreeStack::print(Subtree *startPoint) {
    startPoint->data->print();
    if (startPoint->next != nullptr) {
        print(startPoint->next);
    }
}