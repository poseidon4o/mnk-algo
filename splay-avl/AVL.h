#ifndef _AVL_H_INCLUDED_
#define _AVL_H_INCLUDED_

#include <cmath>
#include <algorithm>

typedef int T;

class AVL : TreeBase<T> {
    struct Node {
        T mData;
        int mHeight;
        Node * mLeft, * mRight;

        Node(const T & value) : mLeft(NULL), mRight(NULL), mHeight(1), mData(value) {}
    };

    Node * mRoot;
public:
    AVL() : mRoot(NULL) {}

    bool Find(const T & value) {
        return find(value, mRoot);
    }

    bool Insert(const T & value) {
        if (!Find(value)) {
            insert(value, mRoot);
            return true;
        }
        return false;
    }

    bool Remove(const T & value) {
        return false;
    }
private:

    int difference(Node * parent) {
        return height(parent->mRight) - height(parent->mLeft);
    }

    int height(Node * node) {
        return node ? node->mHeight : 0;
    }

    void fixheight(Node *& node) {
        node->mHeight = std::max(height(node->mLeft), height(node->mRight)) + 1;
    }

    void left(Node *& parent) {
        Node * target = parent->mLeft;
        parent->mLeft = target->mRight;
        target->mRight = parent;
        
        fixheight(parent);
        fixheight(target);
        
        parent = target;
    }

    void right(Node *& parent) {
        Node * target = parent->mRight;
        parent->mRight = target->mLeft;
        target->mLeft = parent;

        fixheight(parent);
        fixheight(target);

        parent = target;
    }

    void balance(Node *& parent, const T & value) {
        int diff = difference(parent);

        if (diff > 1 && value < parent->mLeft->mData) {
            right(parent);
        } else if (diff < -1 && value > parent->mRight->mData) {
            left(parent);
        } else if (diff > 1 && value > parent->mLeft->mData) {
            left(parent->mLeft);
            right(parent);
        } else if (diff < -1 && value < parent->mRight->mData) {
            right(parent->mRight);
            left(parent);
        }

        
    }

    void insert(const T & value, Node *& node) {
        if (node == NULL) {
            node = new Node(value);
            return;
        }

        if (value < node->mData) {    
            insert(value, node->mLeft);
        } else {
            insert(value, node->mRight);
        }

        fixheight(node);
        balance(node, value);
    }

    bool find(const T & value, Node * node) {
        if (!node) {
            return false;
        }
        if (node->mData == value) {
            return true;
        } else if (value < node->mData) {
            return find(value, node->mLeft);
        } else {
            return find(value, node->mRight);
        }
    }
};

#endif // _AVL_H_INCLUDED_