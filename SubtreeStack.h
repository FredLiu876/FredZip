#ifndef SUBTREESTACK_H
#define SUBTREESTACK_H

#include "node.h"
using namespace std;

// Stack to generate huffman tree from subtrees based on its post order traversal encryption
class SubtreeStack {

    public:
        // Node for stack containing subtrees
        struct Subtree {

            Node *data;
            Subtree *next;

            Subtree();
            ~Subtree();
        };
    
        Subtree *head;

        SubtreeStack();
        ~SubtreeStack();

        unsigned int getSize();
        void push(Node *node);
        /* Removes two of the first subtrees and combines them into larger subtree
        then pushes larger subtree back into stack */
        void pop();
        // For testing purposes only
        void print();

    private:
        unsigned int size;

        void print(Subtree *startPoint);

};

#endif