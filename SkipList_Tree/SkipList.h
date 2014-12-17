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
        T & operator[](int idx) {
            if (idx >= 0) {
                return std::vector<T>::operator[](idx);
            } else {
                return std::vector<T>::operator[](size() + idx);
            }
        }
        const T & operator[](int idx) const {
            if (idx >= 0) {
                return std::vector<T>::operator[](idx);
            } else {
                return std::vector<T>::operator[](size() + idx);
            }
        }

        int height() const {
            return size();
        }
    };


    typedef ExtVector<ListNode *> SearchPath;
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
        for (int c = 0; c < maxHeight; ++c) {
            mHead.mTower.push_back(&mTail);
            mTail.mTower.push_back(NULL);
        }
    }

    bool Insert(const T & value) {
        SearchPath path;
        if (!find(value, path, true)) {
            ListNode * newElement = new ListNode;
            newElement->mData = value;
            ListNode * previous = path[-1];

            // level 0 connections
            newElement->mTower.push_back(previous->mTower[0]);
            previous->mTower[0] = newElement;
            
            int floor = 1;
            while (makeFloor()) {
                int stepBack = -(floor + 1);
                newElement->mTower.push_back(path[stepBack]->mTower[floor]);
                path[stepBack]->mTower[floor] = newElement;
                ++floor;
            }
            return true;
        }
        return false;
    }

    bool Find(const T & value, SearchPath & path = SearchPath()) {
        return find(value, path, true);
    }

    bool Delete(const T & value) {
        SearchPath path;
        if (find(value, path, false)) {
            ListNode * target = path[-1];

            for(int c = 0; c < static_cast<int>(target->mTower.size()); ++c) {
                path[-(c + 1)]->mTower[c] = target->mTower[c];
            }

            delete target;
            return true;
        }
        return false;
    }

    ~SkipList() {
        while (mHead.mTower[0] != &mTail) {
            ListNode * next = mHead.mTower[0]->mTower[0];
            delete mHead.mTower[0];
            mHead.mTower[0] = next;
        }
    }

private:

    bool find(const T & value, SearchPath & path, bool isPathInsert) {
        int shiftDown = -1;
        ListNode * current = &mHead;
        
        path.push_back(current);
        while (-shiftDown <= current->mTower.height()) {
            int lastHeight = current->mTower.height();
            while (-shiftDown <= current->mTower.height() && current->mTower[shiftDown] && 
                (current->mTower[shiftDown]->mData > value ||
                (!isPathInsert && current->mTower[shiftDown]->mData == value)) // if this wont be insert path we want to comapre using >=
            ) {
                --shiftDown;
                path.push_back(current);
            }
            
            if (-shiftDown > current->mTower.height() || current->mTower[shiftDown] == NULL) {
                return false;
            } else {
                current = current->mTower[shiftDown];
                shiftDown = shiftDown + (lastHeight - current->mTower.height());
                shiftDown = -1;
            }
        }
        
        return path[-1]->mData == value;
    }


    bool makeFloor() {
        return rand() & 1;
    }
};

std::ostream & operator<<(std::ostream & output, const SkipList & list) {
    const SkipList::ListNode * current = &list.mHead;
    while (current) {
        output << current->mData;
        for (int c = 0; c < current->mTower.height(); ++c) {
            output << ' ';
            if (current->mTower[c]) {
                output << current->mTower[c]->mData;
            } else {
                output << "000";
            }
        }
        output << std::endl;
        current = current->mTower[0];
    }
    return output;
}

const int SkipList::maxHeight = 20;


#endif // #define SKIPLIST_INCLUDED_H