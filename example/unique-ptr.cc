/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <string>
#include <utility>
#include <memory>
#include <functional>
#include <cstdint>
#include <map>
#include <iostream>

// helper template for numbers to leave defined state after move
template <typename T, T defaultVal>
class DefNumber
{
public:
    DefNumber() = default;
    DefNumber(T v) : val(v) {};
    DefNumber(DefNumber const &) = default;
    DefNumber(DefNumber &&other) : val{other.val} { other.val = defaultVal; }

    // modifying operators
    DefNumber &operator=(DefNumber const &) = default;
    DefNumber &operator=(DefNumber &&other)
    {
        val = other.val;
        other.val = defaultVal;
        return *this;
    }
    DefNumber &operator+=(DefNumber add) { return val += add; }
    DefNumber &operator-=(DefNumber sub) { return val += sub; }
    DefNumber &operator*=(DefNumber mul) { return val += mul; }
    DefNumber &operator/=(DefNumber div) { return val += div; }
    DefNumber &operator%=(DefNumber div) { return val += div; }
    DefNumber operator++(int) { return val++; }
    DefNumber operator++() { return ++val; }
    DefNumber operator--(int) { return val--; }
    DefNumber operator--() { return --val; }

    // conversion to real number
    operator T() const { return val; }

private:
    T val = defaultVal;
};

// helpers for moving 'this'
struct PtrTracker
{
    PtrTracker()
      : oldPtr{nullptr}
      , newPtr{this}
    {}

    PtrTracker(PtrTracker &&other)
      : oldPtr{&other}
      , newPtr{this}
    {
        other.oldPtr = &other;
        other.newPtr = this;
    }

    ~PtrTracker() = default;

    PtrTracker &operator=(PtrTracker &&other)
    {
        oldPtr = &other;
        other.oldPtr = &other;
        newPtr = this;
        other.newPtr = this;

        return *this;
    }

    PtrTracker *oldPtr;
    PtrTracker *newPtr;
};

// move-only variant of StaticVector
template <class T>
class StaticVector : public PtrTracker
{
public:
    explicit StaticVector(int size);

    T *begin()
    {
        return pData;
    }

    T *end()
    {
        return pData + realSize;
    }

    template <typename U>
    T &push_back(U &&t)
    {
        // no checks
        new (pData + realSize) T(std::forward<U>(t));
        return pData[realSize++];
    }

    void pop()
    {
        pData[--realSize].~T();
    }

    T &operator[](int idx)
    {
        return pData[idx];
    }

    int size() const { return realSize; }

private:
    // helper for registering 'this'
    struct ThisRegister
    {
        ThisRegister(StaticVector *vec)
          : owner{vec}
        {}
        ThisRegister(ThisRegister &&other)
          : owner{other.owner ? static_cast<StaticVector *>(other.owner->newPtr)
                              : nullptr}
        {
            if (owner)
            {
                // we're running before the data ctor, so we need to ask
                // still the other for the pointer
                auto oldOwner = static_cast<StaticVector *>(owner->oldPtr);
                owner->registry[oldOwner->data.get()] = owner;
                other.owner = nullptr;
            }
        }
        ~ThisRegister()
        {
            if (owner)
            {
                owner->registry.erase(owner->data.get());
            }
        }

        ThisRegister &operator=(ThisRegister &&other)
        {
            if (this != &other)
            {
                this->~ThisRegister();
                new (this) ThisRegister{std::move(other)};
            }
            return *this;
        }

        StaticVector *owner;
    };

    DefNumber<int, 0> dataSize;
    DefNumber<int, 0> realSize;

    ThisRegister moveHelper; // needs to be deleted after data!
    //std::unique_ptr<uint8_t, std::function<void(uint8_t *)>> data;
    //std::unique_ptr<uint8_t, decltype(deleter)> data; // not known yet
    std::unique_ptr<uint8_t, void(*)(uint8_t *)> data;
    T *pData;

    static std::map<uint8_t *, StaticVector *> registry;
    void foo() { std::cout << ',' << realSize << '\n'; }
#if __cplusplus >= 201703
    static constexpr auto deleter =
                     [] (uint8_t *ptr)
                     {
                         StaticVector *me = registry[ptr];
                         // destruct all elements
                         for (int i = 0; i != me->realSize; ++i)
                         {
                             me->pData[i].~T();
                         }
                         // free memory
                         delete [] ptr;
                     };
#else
    static void deleter(uint8_t *ptr)
    {
        StaticVector *me = registry[ptr];
        // destruct all elements
        for (int i = 0; i != me->realSize; ++i)
        {
            me->pData[i].~T();
        }
        // free memory
        delete [] ptr;
    }
#endif
};

template <typename T>
std::map<uint8_t *, StaticVector<T> *> StaticVector<T>::registry;

template <typename T>
StaticVector<T>::StaticVector(int size)
  : dataSize{size}
  , moveHelper{this}
  , data{new uint8_t[size*sizeof(T)], deleter}
  , pData{static_cast<T *>((void *)(data.get()))}
{
    registry[data.get()] = this;
}

struct TestInt
{
    TestInt(int i_) : i{i_} { std::cout << "TestInt(int): " << this << "\n"; }
    ~TestInt() { std::cout << "~TestInt(): " << this << "\n"; }

    operator int() const { return i; }

    int i;
};

int main()
{
    StaticVector<TestInt> sv{17};

    sv.push_back(5);
    std::cout << sv.size() << '\n';
    std::cout << sv[0] << '\n';

    StaticVector<TestInt> sv2{std::move(sv)};
    std::cout << sv.size() << '\n';
    std::cout << sv2.size() << '\n';
    sv2.push_back(17);

    for (auto i: sv2)
    {
        std::cout << i.i << ' ';
    }

    sv2.pop();
    std::cout << sv2.size() << '\n';

    return 0;
}
