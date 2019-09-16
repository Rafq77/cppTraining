/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <string>
#include <utility>
#include <type_traits>
#include <iostream>

class Person
{
public:
    Person() = default;               // default constructor

    Person(Person const&) = delete;  // copy constructor
    Person(Person &&rhs)              // move constructor
      : name(std::move(rhs.name))
      , role(std::move(rhs.role))
    {}

#if 0
    Person(std::string const &name_, std::string const &role_)
      : name{name_}
      , role{role_}
    {}
    Person(std::string &&name_, std::string &&role_)
      : name{std::move(name_)}
      , role{std::move(role_)}
    {}
#else
    template <class STR1, class STR2>
    Person(STR1 &&name_, STR2 &&role_)
      : name{std::forward<STR1>(name_)}
      , role{std::forward<STR2>(role_)}
    {}
#endif
    ~Person() = default;

    Person &operator=(Person const&) = default;  // copy assignment
    Person &operator=(Person &&rhs) = default;   // move assignment

#if 0
    void setName(std::string const &n)           // copy name in
    {
        name = n;
    }
    void setName(std::string &&n)                // move name in
    {
        name = std::move(n);
    }
#else
    void setName(std::string n)                  // take all
    {
        std::swap(name, n);
    }
#endif

    template <class STR>
    void setRole(STR &&r)
    {
        role = std::forward<STR>(r);
    }

    std::ostream &print(std::ostream &o) const
    {
        return o << "Name: " << name << ", Role: " << role;
    }

private:
    std::string name;
    std::string role;
};
std::ostream &operator<<(std::ostream &os, Person const &p)
{
    return p.print(os);
}

template <class T>
class StaticVector
{
    static_assert(std::is_trivially_destructible<T>::value == true,
                  "T must be trivially destructible");
public:
    explicit StaticVector(int size)
      : dataSize(size)
      , data(new T[size])
    {}

    StaticVector(StaticVector const &rhs)
      : dataSize(rhs.dataSize)  // copy size
      , data(new T[dataSize])          // copy pointer
    {
        // ... (copy construct all existing members from rhs)
    }

    StaticVector(StaticVector &&rhs)
      : dataSize(rhs.dataSize)  // copy size
      , data(rhs.data)          // copy pointer
    {
        rhs.data = nullptr;  // invalidate other data
    }

    ~StaticVector() {
        delete [] data;
    }

    StaticVector &operator=(StaticVector const &rhs)
    {
        StaticVector tmp(rhs); // copy construction
        std::swap(data, tmp.data);
        std::swap(dataSize, tmp.dataSize);

        return *this;
    }                          // destruct old value

    StaticVector &operator=(StaticVector &&rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        T *oldData = data;

        data = rhs.data;
        dataSize = rhs.dataSize;

        rhs.data = nullptr;

        // do this last, it might throw exceptions
        delete [] oldData;

        return *this;
    }

private:
    int dataSize;
    T *data;
};

int main()
{
    std::string n = "Asterix";
    std::string r = "The Gaul";
    Person a(n, r);
    Person b("Billy", "The Kid");
    std::cout << a << '\n';
    std::cout << b << '\n';
    b.setRole("Cowboy");
    std::cout << b << '\n';
    return 0;
}
