#ifndef _SPLAY_H_INCLUDED_
#define _SPLAY_H_INCLUDED_

#include "TreeBase.h"

#include <cstdint>
#include <cstdlib>
#include <cassert>

template <typename T>
class SplayTree : TreeBase<T> {

    class MoveData {
    public:
        enum path_t : uint8_t {
            LEFT = 1,                          // 0001
            RIGHT = 1 << 1,                    // 0010
            LEFT_LEFT = 1 | (1 << 2),          // 0101
            RIGHT_RIGHT = (1 << 3) | (1 << 1), // 1010
            LEFT_RIGHT = (1 << 2) | (1 << 1),  // 0110
            RIGHT_LEFT = (1 << 3) | 1          // 1001
        };
    private:
        union {
            struct {
                bool mBad : 1;
                path_t mPath : 4;
                uint8_t mLevel : 3;
            };
            uint8_t mData;
        };

        uint8_t inc() {
            return ++mLevel;
        }
    public:

        MoveData() : mData(0) {}

        MoveData & Bad(bool) {
            mBad = true;
            return *this;
        }

        bool Bad() {
            return mBad;
        }

        int Level() const {
            return mLevel;
        }

        // return true if right
        bool operator[](int idx) {
            assert(idx >= 0 && idx < 2);
            return (mPath >> idx) & 0x1;
        }

        MoveData & GoLeft() {
            uint8_t lvl = inc();
            if (lvl == 1) {
                mPath = LEFT;
            } else {
                if (mPath == LEFT) {
                    mPath = LEFT_LEFT;
                } else {
                    mPath = RIGHT_LEFT;
                }
            }
            return *this;
        }

        path_t Path() const {
            return mPath;
        }

        MoveData & GoRight() {
            uint8_t lvl = inc();
            if (lvl == 1) {
                mPath = RIGHT;
            } else {
                if (mPath == LEFT) {
                    mPath = LEFT_RIGHT;
                } else {
                    mPath = RIGHT_RIGHT;
                }
            }
            return *this;
        }

        MoveData & Clear() {
            mData = 0;
            return *this;
        }
    };

    struct Node {
        T mData;
        Node * mLeft, * mRight;
    };

    Node * mRoot;


public:
    SplayTree() : mRoot(NULL) {
    }

    bool Find(const T & value) override {
        MoveData data = find(value, mRoot);
        splay(mRoot, data, true);
        return !data.Bad();
    }

    bool Insert(const T & value) override {
        MoveData data = insert(value, mRoot);
        splay(mRoot, data, true);
        return !data.Bad();
    }

    bool Remove(const T & value) override {
        // 1. splay value to root
        // 2. remove root
        // 3. join left and right subtrees
        if (Find(value)) {
            if (mRoot->mLeft) {
                const T * newValue = find_max(mRoot->mLeft);
                MoveData data = find(*newValue, mRoot->mLeft);
                splay(mRoot->mLeft, data, true);

                Node * newRoot = mRoot->mLeft;
                newRoot->mRight = mRoot->mRight;
                delete mRoot;
                mRoot = newRoot;
            } else if (mRoot->mRight) {
                const T * newValue = find_min(mRoot->mRight);
                MoveData data = find(*newValue, mRoot->mRight);
                splay(mRoot->mRight, data, true);

                Node * newRoot = mRoot->mRight;
                newRoot->mLeft = mRoot->mLeft;
                delete mRoot;
                mRoot = newRoot;
            } else {
                delete mRoot;
                mRoot = NULL;
            }

            return true;
        }
        return false;
    }

private:

    const T * find_min(Node * root) {
        return root->mLeft ? find_min(root->mLeft) : &root->mData;
    }

    const T * find_max(Node * root) {
        return root->mRight ? find_min(root->mRight) : &root->mData;
    }

    void zig(Node *& parent) {
        Node * target = parent->mLeft;
        parent->mLeft = target->mRight;
        target->mRight = parent;
        parent = target;
    }

    void zag(Node *& parent) {
        Node * target = parent->mRight;
        parent->mRight = target->mLeft;
        target->mLeft = parent;
        parent = target;
    }

    MoveData splay(Node *& n, MoveData data, bool force = false) {
        if (data.Bad() || data.Level() == 0) {
            return data;
        }

        if (data.Level() < 2 && !force) {
            return data;
        }

        switch (data.Path())
        {
        case SplayTree::MoveData::LEFT:
            zig(n);
            break;
        case SplayTree::MoveData::RIGHT:
            zag(n);
            break;
        case SplayTree::MoveData::LEFT_LEFT:
            zig(n->mLeft);
            zig(n);
            break;
        case SplayTree::MoveData::RIGHT_RIGHT:
            zag(n->mRight);
            zag(n);
            break;
        case SplayTree::MoveData::LEFT_RIGHT:
            zig(n->mRight);
            zag(n);
            break;
        case SplayTree::MoveData::RIGHT_LEFT:
            zag(n->mLeft);
            zig(n);
            break;
        }

        return data.Clear();
    }

    MoveData insert(const T & value, Node *& n) {
        if (!n) {  
            n = new Node();
            n->mLeft = n->mRight = NULL;
            n->mData = value;
            return MoveData();
        }
        MoveData data;
        if (n->mData == value) {
            return MoveData().Bad(true);
        } else if (value < n->mData) {
            data = insert(value, n->mLeft);
            data.GoLeft();
        } else {
            data = insert(value, n->mRight);
            data.GoRight();
        }

        return splay(n, data);
    }

    MoveData find(const T & value, Node *& n) {
        if (!n) {
            return MoveData().Bad(true);
        }
        MoveData data;
        if (n->mData == value) {
            return MoveData();
        } else if (value < n->mData) {
            data = find(value, n->mLeft);
            data.GoLeft();
        } else {
            data = find(value, n->mRight);
            data.GoRight();
        }

        return splay(n, data);
    }
};

#endif // _SPLAY_H_INCLUDED_