#ifndef INDEXLIST_H
#define INDEXLIST_H

// Linked list containing all indexes of a character in a string for LZ77 compression
class IndexList {
    public:
        // Node for IndexList linked list
        struct IndexNode {
            IndexNode *next;
            unsigned int index;
            IndexNode();
            ~IndexNode();
        };
        IndexNode *head;
        IndexNode *tail;
        IndexList();
        ~IndexList();
        void push(unsigned int newIndex);
        void print();
        unsigned int getSize() const;
    private:
        unsigned int size;
};

#endif