#ifndef SKIPLIST_INCLUDED_H
#define SKIPLIST_INCLUDED_H

#include <list>
#include <vector>
#include <random>
#include <ios>
#include <algorithm>
#include <functional>

class SkipList {
    typedef int T;
    struct ListNode;

    template<typename T>
    struct ExtVector: std::vector<T> {
         
        // support negative indexing
        ListNode *& operator[](int idx) {
            if (idx >= 0) {
                return this->operator[](idx);
            } else {
                return this->operator[](size() - idx - 1);
            }
        }
        ListNode * operator[](int idx) const {
            if (idx >= 0) {
                return this->operator[](idx);
            } else {
                return this->operator[](size() - idx - 1);
            }
        }

        int height() const {
            return size();
        }
    };


    typedef ExtVector<const ListNode *> SearchPath;
    struct ListNode {
        T mData;
        ExtVector<ListNode*> mTower;
    };

    ListNode mHead, mTail;
    static const int maxHeight;

    friend std::ostream & operator<<(std::ostream & output, const SkipList & list);
public:
    SkipList(const T & min, const T & max) {
        mHead.mData = min;
        mTail.mData = max;
    }

    void Insert(const T & value) {
        SearchPath path;
        if (!this->Find(value, path)) {
            ListNode * newElement = new ListNode;
            newElement->mData = value;
            ListNode * previous = path[-1];

            // level 0 connections
            newElement->mTower.push_back(previous->mTower[0]);
            previous->mTower[0] = newElement;
            
            int floor = 1;
            while (makeFloor()) {
                int stepBack = -(floor + 1);
                newElement->mTower.push_back(path[floor]->mTower[floor]);
                path[floor]->mTower[floor] = newElement;
                ++floor;
            }
        }

    }

    bool Find(const T & value, SearchPath & path = SearchPath()) const {
        int shift_down = -mHead.mTower.size();
        const ListNode * current = mHead.mTower[shift_down];
        
        path.push_back(current);
        while (-shift_down >= 0) {
            while (current->mTower[shift_down]->mData > value) {
                --shift_down;
                path.push_back(current);
            }
            current = current->mTower[shift_down];
        }
        
        return path[-1]->mData == value;        
    }

    void Delete(const T & value) {
        SearchPath path;
        if (this->Find(value, path)) {

        }
    }

    ~SkipList() {
        while (mHead.mTower[0] != &mTail) {
            ListNode * next = mHead.mTower[0]->mTower[0];
            delete mHead.mTower[0];
            mHead.mTower[0] = next;
        }
    }

private:
    bool makeFloor() {
        return false;
    }
};

std::ostream & operator<<(std::ostream & output, const SkipList & list) {
    const SkipList::ListNode * current = &list.mHead;
    while (current->mTower[0]) {
        for (int c = 0; c < current->mTower.height(); ++c) {
            output << current->mTower[c]->mData << "[" << current->mTower[0] << "] ";
        }
        output << std::endl;
    }
    return output;
}

template<typename T>
const int SkipList<T>::maxHeight = 20;


#endif // #define SKIPLIST_INCLUDED_H