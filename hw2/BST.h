#ifndef BST_INCLUDED_H
#define BST_INCLUDED_H

#include <ios>
#include <iomanip>
#include <queue>

template <typename T>
class BST {
    //typedef int T;
    struct node {
        node * mLeft, * mRight;
        T mData;
        node(const T & d): mLeft(NULL), mRight(NULL), mData(d)  {}
    };

    int mElementCount;
    node * mRoot;

    template <typename T>
    friend std::ostream & operator<<(std::ostream & strm, const BST<T> & bst);
public:
    BST(): mElementCount(0), mRoot(NULL) {

    }

    int Size() const {
        return mElementCount;
    }

    bool Find(const T & value) {
        return NULL != find(value);
    }

    bool Delete(const T & value) {
        node *& where = find(value);
        if (where) {
            if (where->mLeft && where->mRight) {
                node * swp = getMin(where->mLeft);
                where->mData = swp->mData;
                delete swp;
            } else if (where->mLeft) {
                node * swp = where;
                where = where->mLeft;
                delete swp;
            } else if (where->mRight) {
                node * swp = where;
                where = where->mRight;
                delete swp;
            } else {
                delete where;
                where = NULL;
            }
            return true;
        }
        return false;
    }

    bool Insert(const T & value) {
        node *& where = find(value);
        if (!where) {
            where = new node(value);
            return true;
        }
        return false;
    }

    ~BST() {
        std::queue<node *> que;
        que.push(mRoot);

        while (!que.empty()) {
            node * n = que.front();
            que.pop();
            if (n) {
                que.push(n->mLeft);
                que.push(n->mRight);
                delete n;
            }
        }
    }

private:
    void print(std::ostream & out, node * n, int depth = 1) const {
        if (n) {
            print(out, n->mLeft, depth + 1);
            out << n->mData << ' ';
            print(out, n->mRight, depth + 1);
        }
    }

    node * getMin(node * n) {
        node * next = n->mLeft,* cur = n;

        while (next->mLeft) {
            cur = next;
            next = next->mLeft;
        }

        cur->mLeft = next->mRight;
        return next;
    }

    node *& find(const T & value) {
        node ** n = &mRoot;
        while (*n) {
            if (value < (*n)->mData) {
                n = &(*n)->mLeft;
            } else if (value >(*n)->mData) {
                n = &(*n)->mRight;
            } else {
                return *n;
            }
        }

        return *n;
    }

};

template <typename T>
std::ostream & operator<<(std::ostream & strm, const BST<T> & bst) {
    bst.print(strm, bst.mRoot);
    return strm;
}

#endif // BST_INCLUDED_H