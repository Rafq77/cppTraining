/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef GROUP_TEST_HH_SEEN_
#define GROUP_TEST_HH_SEEN_

#ifdef SPECIAL_VECTOR_MOCK
#include <vector>
namespace std
{
template <typename T>
using OrigVector = vector<T>;

#define vector VectorMock

template <typename T>
struct VectorMock : OrigVector<T>
{
    VectorMock()
    {
        lastVector = this;
    }

    // specializing the move ctor (of VectorMock) via enable_if isn't easy:
    //  - we don't have a return type
    //  - we can't add another template parameter
    //  - we can't add a parameter
    //  - we can't just call another function, as we want to initialize
    // using constexpr if is easier, but not available for C++11/14
    // so we use a special constructor with an additional argument
    // and forward to it
    VectorMock(VectorMock &&other)
      : VectorMock<T>::VectorMock(std::move(other), 0)
    {}

    template <class X>
    VectorMock(X &&other
               , typename std::enable_if<std::is_copy_constructible<typename X::value_type>::value, int>::type)
      : OrigVector<T>{other}
    {}
    template <class X>
    VectorMock(X &&other
               , typename std::enable_if<!std::is_copy_constructible<typename X::value_type>::value, int>::type)
      : OrigVector<T>{std::move(other)}
    {}

    VectorMock(VectorMock const &other) = default;
    VectorMock &operator=(VectorMock &&other)
    {
        VectorAssign(*this, other);
        return *this;
    }
    VectorMock &operator=(VectorMock const &other) = default;

    static VectorMock *lastVector;
};

template <typename T>
VectorMock<T> *VectorMock<T>::lastVector = nullptr;

// different assignments for VectorMock for copyable and move-only members
template <typename T>
typename std::enable_if<std::is_copy_constructible<T>::value>::type
VectorAssign(std::OrigVector<T> &lhs, std::OrigVector<T> &rhs)
{
    lhs = rhs;
}
template <typename T>
typename std::enable_if<!std::is_copy_constructible<T>::value>::type
VectorAssign(std::OrigVector<T> &lhs, std::OrigVector<T> &rhs)
{
    lhs = std::move(rhs);
}
} // namespace std
#endif // SPECIAL_VECTOR_MOCK
#endif /* GROUP_TEST_HH_SEEN_ */
