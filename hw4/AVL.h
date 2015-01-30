#ifndef _AVL_H_INCLUDED_
#define _AVL_H_INCLUDED_

typedef int T;

class AVL : TreeBase<T> {
    struct Node {
        T mData;
        int mHeight;
        Node * mLeft, * mRight;

        Node(const T & value) : mLeft(NULL), mRight(NULL), mHeight(0), mData(value) {}
    };

    Node * mRoot;
public:
    AVL() : mRoot(NULL) {}

    bool Find(const T & value) {
        return find(value, mRoot);
    }

    bool Insert(const T & value) {

    }

private:

    void insert(const T & value, Node *& node) {
        if (node == NULL) {
            node = new Node(value);
        }
        ++node->mHeight;

        if (value < node->mData) {    

        }
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