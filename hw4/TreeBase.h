#ifndef _TREE_BASE_H_INCLUDED_
#define _TREE_BASE_H_INCLUDED_


template <typename T>
class TreeBase {
public:
    virtual bool Find(const T &) = 0;
    virtual bool Remove(const T &) = 0;
    virtual bool Insert(const T &) = 0;
};


#endif // _TREE_BASE_H_INCLUDED_