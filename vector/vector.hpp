#ifndef _VECTOR_H_INCLUDED_
#define _VECTOR_H_INCLUDED_

#include <new>
#include <type_traits>
#include <assert.h>
#include <vector>

template <typename T>
class vector {


    T *mFrist, *mLast, *mCurrent;
public:
    vector(int size = 0) : mFrist(nullptr), mLast(nullptr), mCurrent(nullptr) {
        resize(size);
    }

    std::enable_if<std::is_copy_constructible<T>::value, void>::type
        push_back(const T & val) {
        
        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent++)T(val);
    }

    std::enable_if<std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value, void>::type
        push_back(const T & val) {

        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent)T();
        *mCurrent = val;
        ++mCurrent;
    }

    template <typename ...T>
    void emplace_back(T && ... args) {
        resize(size() + 1);

        assert(mCurrent <= mLast);

        new(mCurrent++)T(std::forward<T>(args));
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

        if (newSize < capacity()) {
            return;
        }

        assert(newSize > size());

        T * newRange = operator new(newSize);
        move_range(newRange);
        mCurrent = newRange + size();
        mLast = newRange + newSize;

        delete[] mFrist;
        mFrist = newRange;
    }

    typename std::enable_if<std::is_pod<T>::value, void>::type
        move_range(T * newStart) {
        
        memcpy(newStart, mFrist, size());
    }

    typename std::enable_if<!std::is_pod<T>::value, void>::type
        move_range(T * newStart) {

        T * iter = mFrist;
        for (int c = size(); c >= 0; --c) {
            *(newStart++) = *(iter++);
        }
    }



};


#endif // _VECTOR_H_INCLUDED_