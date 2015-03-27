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
    using CanDefaultAssign = typename bool_type<!std::is_copy_constructible<Q>::value && std::is_default_constructible<Q>::value && std::is_copy_assignable<Q>::value>;

    template <typename Q>
    using CanCopyCtor = typename bool_type<std::is_copy_constructible<Q>::value && !std::is_default_constructible<Q>::value || std::is_pod<Q>::value>;

    template <typename Q>
    using CanMemcpy = typename bool_type<std::is_pod<Q>::value || std::is_trivially_copyable<Q>::value>;

    T *mFrist, *mLast, *mCurrent;
public:
    vector(int size = 0) : mFrist(nullptr), mLast(nullptr), mCurrent(nullptr) {
        resize(size);
    }

    ~vector() {
        delete[] mFrist;
    }



    template <typename Q = T>
    void push_back(const typename std::enable_if<CanCopyCtor<Q>::value, Q>::type & val) {

        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent++)Q(std::move(val));
    }

    template <typename Q = T>
    void push_back(const typename std::enable_if<CanDefaultAssign<Q>::value, Q>::type & val) {

        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent)Q();
        mCurrent->operator=(std::move(val));

        ++mCurrent;
    }

    template <typename ...Q>
    void emplace_back(Q && ...args) {
        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent++)T(std::forward<Q...>(args...));
    }

    void pop_back() {
        assert(mCurrent > mFrist);
        mCurrent->~T();
        --mCurrent;
    }


    int size() const {
        return mCurrent - mFrist;
    }

    int capacity() const {
        return mLast - mFrist;
    }

private:

    void resize(int newSize) {

        if (newSize <= capacity()) {
            return;
        }

        assert(newSize > size());

        T * newRange = reinterpret_cast<T*>(operator new(newSize * sizeof(T)));
        move_range(newRange);

        mCurrent = newRange + size();
        mLast = newRange + newSize;

        delete[] mFrist;
        mFrist = newRange;
    }


    template <typename Q = T>
    void move_range(typename std::enable_if<CanMemcpy<Q>::value, Q>::type * newStart) {
        memcpy(newStart, mFrist, size() * sizeof(Q));
    }

    template <typename Q = T>
    void move_range(typename std::enable_if<!CanMemcpy<Q>::value && CanCopyCtor<Q>::value, Q>::type * newStart) {

        Q * iter = mFrist;
        for (int c = size(); c >= 0; --c) {
            new(newStart++)Q(std::move(*(iter++)));
        }
    }

    template <typename Q = T>
    void move_range(typename std::enable_if<!CanMemcpy<Q>::value && !CanCopyCtor<Q>::value && CanDefaultAssign<Q>::value, Q>::type * newStart) {

        Q * iter = mFrist;
        for (int c = size(); c >= 0; --c) {
            new(newStart)Q();
            *(newStart++) = std::move(*(iter++));
        }
    }
};

#endif // _VECTOR_H_INCLUDED_
