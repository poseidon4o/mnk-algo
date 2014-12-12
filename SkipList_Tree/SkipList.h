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
            ListNode * elem = new ListNode;
            elem->mData = value;

            elem->mTower.push_back(path[-1]->mTower[0]); // point to next, floor = 1
            path[-1]->mTower[0] = elem; // prev point to me

            int floor = 1;
            ListNode * next = elem->mTower[0];
            while (makeFloor()) {
                next = atDistance(next, 1 << (floor-1));
                //next = atDistance(elem, 1 << floor);
                elem->mTower.push_back(next);
                ++floor;
            }
            for_each(elem->mTower.begin(), elem->mTower.end(), std::mem_fn(&SkipList::fixTower));
        }

    }

    bool Find(const T & value, SearchPath & path = SearchPath()) const {
        int shift_down = -mHead.mTower.size();
        const ListNode * current = mHead.mTower[shift_down];
        path.push_back(current);

        while (-shift_down <= current->mTower.height()) {
            while (value > current->mTower[shift_down]->mData) {
                current = current->mTower[shift_down];
            }
            if (path[-1] != current) {
                path.push_back(current);
            }
            ++shift_down; // go down a floor
        }
        return current->mData == value;
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

    void fixTower(ListNode * target) {
        int floor = 0;
        std::transform(target->mTower.begin(), target->mTower.end(), target->mTower.begin(), [&floor, this] (ListNode *& next) {
            return this->atDistance(next, 1 << floor++);
        });
    }

    ListNode * atDistance(ListNode * source, int dist) {
        while (dist) {
            int floor = 0;
            while (dist > (1 << floor) && floor < source->mTower.height()) {
                ++floor;
            }
            dist -= floor;
            source = source->mTower[floor];
        }
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