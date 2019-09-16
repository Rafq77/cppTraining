#ifndef STATIC_VECTOR_HH_SEEN
#define STATIC_VECTOR_HH_SEEN
/*
 * Copyright (c) 2014-2019 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <cstddef>
#include <initializer_list>

namespace exercise
{
template <class T>
class StaticVector
{
public:
    explicit StaticVector(size_t size);
    explicit StaticVector(std::initializer_list<T> const &);
    StaticVector(StaticVector const &rhs);
    StaticVector(StaticVector &&rhs);

    ~StaticVector();

    StaticVector &operator=(StaticVector tmp); // universal assignment

    T &operator[](size_t idx);
    T const &operator[](size_t idx) const;

    T *begin();
    T const *begin() const;
    T *end();
    T const *end() const;

    void checkSizeAndThrow(size_t idx) const;

    size_t size() const;

    bool equal(StaticVector const &other) const;

private:
    size_t dataSize;
    T *data;
};

template <class T>
bool operator==(StaticVector<T> const &, StaticVector<T> const &);
} // namespace exercise
#endif /* STATIC_VECTOR_HH_SEEN */
