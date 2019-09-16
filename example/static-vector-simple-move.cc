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

// move-only variant of StaticVector
template <class T>
class StaticVector
{
    struct DataHolder
    {
        long long realSize;
        T data[];
    };

public:
    explicit StaticVector(int size);

    T *begin()
    {
        return data->data;
    }

    T *end()
    {
        return data->data + data->realSize;
    }

    template <typename U>
    T &push_back(U &&t)
    {
        // no checks
        new (data->data + data->realSize) T(std::forward<U>(t));
        return data->data[(data->realSize)++];
    }

    void pop()
    {
        data->data[--(data->realSize)].~T();
    }

    T &operator[](int idx)
    {
        return data->data[idx];
    }

    int size() const
    {
        if (data)
        {
            return data->realSize;
        }
        else
        {
            return -1;
        }
    }

private:
    int dataSize;

    std::unique_ptr<DataHolder, void(*)(DataHolder *)> data;

    static void deleter(DataHolder *ptr)
    {
        for (int i = 0; i != ptr->realSize; ++i)
        {
            ptr->data[i].~T();
        }
        // free memory
        uint8_t *tmp = reinterpret_cast<uint8_t *>(ptr);
        delete [] tmp;
    }
};

template <typename T>
StaticVector<T>::StaticVector(int size)
  : dataSize{size}
  , data{nullptr, deleter}
{
    uint8_t *tmp = new uint8_t[sizeof(int) + sizeof(T)*size];
    data.reset(reinterpret_cast<DataHolder *>(tmp));
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

    for (auto &i: sv2)
    {
        std::cout << i.i << ' ';
    }
    std::cout << '\n';

    sv2.pop();
    std::cout << sv2.size() << '\n';

    return 0;
}
