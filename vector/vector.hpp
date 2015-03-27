#ifndef _VECTOR_H_INCLUDED_
#define _VECTOR_H_INCLUDED_

#include <new>
#include <type_traits>
#include <assert.h>


#if _MSC_VER == 1800
//#error MSVC 2013 does NOT support is_copy_constructible for deleted functions
#endif


template <bool Q>
struct bool_type {
    enum { value = Q };
};


template <typename T>
class vector {

    template <typename Q>
    using CanDefaultAssign = bool_type<!std::is_copy_constructible<Q>::value && std::is_default_constructible<Q>::value && std::is_copy_assignable<Q>::value>;

    template <typename Q>
    using CanCopyCtor = bool_type<std::is_copy_constructible<Q>::value || std::is_pod<Q>::value>;

    template <typename Q>
    using CanMemcpy = bool_type<std::is_pod<Q>::value || std::is_trivially_copyable<Q>::value>;

    T *mFirst, *mLast, *mCurrent;
public:
    vector(int size = 0) : mFirst(nullptr), mLast(nullptr), mCurrent(nullptr) {
        resize(size);
    }

    ~vector() {
        for (T * iter = mFirst; iter < mCurrent; ++iter) {
            iter->~T();
        }

        operator delete(mFirst);
    }

    vector(const vector<T> & other): vector(other.size()) {
        other.copy_range(mFirst);
        mCurrent = mFirst + other.size();
    }

    vector & operator=(const vector<T> & other) {
        if (this == &other) {
            return *this;
        }

        for (T * iter = mFirst; iter < mCurrent; ++iter) {
            iter->~T();
        }
        
        mCurrent = mFirst;
        resize(other.size());
        other.copy_range(mFirst);
        mCurrent = mFirst + other.size();

        return *this;
    }

    vector(vector<T> && other): vector(0) {
        std::swap(mFirst, other.mFirst);
        std::swap(mCurrent, other.mCurrent);
        std::swap(mLast, other.mLast);
    }

    vector & operator=(vector<T> && other) {
        std::swap(mFirst, other.mFirst);
        std::swap(mCurrent, other.mCurrent);
        std::swap(mLast, other.mLast);
    }

    template <typename Q = T>
    void push_back(const typename std::enable_if<CanCopyCtor<Q>::value, Q>::type & val) {

        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent++)Q(val);
    }

    template <typename Q = T>
    void push_back(const typename std::enable_if<CanDefaultAssign<Q>::value, Q>::type & val) {

        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent)Q();
        mCurrent->operator=(val);

        ++mCurrent;
    }

    // should work, no idea...
    /*
    template <typename ...ArgsTypes>
    void emplace_back(ArgsTypes && ...args) {
        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent++)T(std::forward<ArgsTypes>(args)...);
    }
    */

    void pop_back() {
        assert(mCurrent > mFirst);
        mCurrent->~T();
        --mCurrent;
    }

    int size() const {
        return mCurrent - mFirst;
    }

    int capacity() const {
        return mLast - mFirst;
    }

    const T & operator[](int idx) const {
        return mFirst + idx;
    }

    T & operator[](int idx) {
        return mFirst + idx;
    }

private:

    void resize(int newSize) {

        if (newSize <= capacity()) {
            return;
        }

        assert(newSize > size());

        T * newRange = reinterpret_cast<T*>(operator new(newSize * sizeof(T)));

        move_range(newRange);

        for (T * iter = mFirst; iter < mCurrent; ++iter) {
            iter->~T();
        }

        mCurrent = newRange + size();
        mLast = newRange + newSize;

        operator delete(mFirst);
        mFirst = newRange;
    }


    // copy data

    template <typename Q = T>
    void copy_range(typename std::enable_if<CanMemcpy<Q>::value, Q>::type * newStart) const {
        move_range(newStart);
    }

    template <typename Q = T>
    void copy_range(typename std::enable_if<!CanMemcpy<Q>::value && CanCopyCtor<Q>::value, Q>::type * newStart) const {

        for (Q * iter = mFirst; iter < mCurrent; ++iter) {
            new(newStart++)Q(*iter);
        }
    }

    template <typename Q = T>
    void copy_range(typename std::enable_if<!CanMemcpy<Q>::value && !CanCopyCtor<Q>::value && CanDefaultAssign<Q>::value, Q>::type * newStart) const {

        for (Q * iter = mFirst; iter < mCurrent; ++iter) {
            new(newStart)Q();
            newStart->operator=(*iter);
        }
    }


    // move data

    template <typename Q = T>
    void move_range(typename std::enable_if<CanMemcpy<Q>::value, Q>::type * newStart) {
        memcpy(newStart, mFirst, size() * sizeof(Q));
    }

    template <typename Q = T>
    void move_range(typename std::enable_if<!CanMemcpy<Q>::value && CanCopyCtor<Q>::value, Q>::type * newStart) {

        for (Q * iter = mFirst; iter < mCurrent; ++iter) {
            new(newStart++)Q(std::move(*iter));
        }
    }

    template <typename Q = T>
    void move_range(typename std::enable_if<!CanMemcpy<Q>::value && !CanCopyCtor<Q>::value && CanDefaultAssign<Q>::value, Q>::type * newStart) {

        for (Q * iter = mFirst; iter < mCurrent; ++iter) {
            new(newStart)Q();
            *(newStart++) = std::move(*(iter++));
        }
    }
};

#endif // _VECTOR_H_INCLUDED_
