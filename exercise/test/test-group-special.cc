/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "lest.hpp"

#include <iostream>
#include <type_traits>
#include <utility>

#define SPECIAL_VECTOR_MOCK
#include "group.hh"

using namespace exercise;
using std::pair;

namespace
{
struct DummyShape : Shape
{
    void doDraw(cairo_t *context) const
    {
    }

};

// below we need VectorMock, but it should be ok if it doesn't exist
// so we use a trait from
// <https://stackoverflow.com/questions/10711952/how-to-detect-existence-of-a-class-using-sfinae>
template< typename T>
struct has_destructor
{   
    template <typename A> 
    static std::true_type test(decltype(std::declval<A>().~A()) *) {
        return std::true_type();
    }

    template<typename A>
    static std::false_type test(...) {
        return std::false_type(); 
    }

    typedef decltype(test<T>(0)) type;

    static const bool value = type::value;
};
} // unnamed namespace

namespace std
{
template <typename T>
struct VectorMock;
}
using std::VectorMock;

namespace exerciseTest
{
#ifdef HAS_GROUP_MOVE_CTOR
template <class G, class V>
typename std::enable_if<std::is_copy_constructible<typename G::testElemType>::value, bool>::type
testGroupMoveCtor(G *from, G *to)
{
    new (from) G;
    V *fromVec = V::lastVector;
    auto s = new DummyShape;
    from->addChild(s);

    new (to) G{std::move(*from)};

    return fromVec->empty();
}
template <class G, class V>
typename std::enable_if<!std::is_copy_constructible<typename G::testElemType>::value, bool>::type
testGroupMoveCtor(G *from, G *to)
{
    new (from) G;
    new (to) G;

    return true;
}
#endif // HAS_GROUP_MOVE_CTOR

#ifdef HAS_GROUP_MOVE_ASSIGNMENT
template <class G, class V>
typename std::enable_if<std::is_copy_constructible<typename G::testElemType>::value, bool>::type
testGroupMoveAss(G *from, G *to)
{
    new (from) G;
    V *fromVec = V::lastVector;
    auto s = new DummyShape;
    from->addChild(s);
    new (to) G;

    *to = std::move(*from);

    return fromVec->empty();
}

template <class G, class V>
typename std::enable_if<!std::is_copy_constructible<typename G::testElemType>::value, bool>::type
testGroupMoveAss(G *from, G *to)
{
    new (from) G;
    new (to) G;

    return true;
}
#endif // HAS_GROUP_MOVE_ASSIGNMENT

const lest::test groupSpecialTests[] =
{
#ifdef HAS_GROUP_MOVE_CTOR
    {CASE("VectorMock move must copy")
     {
         VectorMock<int> v1;
         VectorMock<int> *v1p = VectorMock<int>::lastVector;
         v1.push_back(5);
         v1.push_back(8);
         VectorMock<int> v2{std::move(v1)};

         VectorMock<int> v3;
         VectorMock<int> *v3p = VectorMock<int>::lastVector;
         v3 = std::move(v2);

         EXPECT(v1p->size() == v3p->size());
     }},
    {CASE("Group move ctor must empty moved-from object")
     {
         unsigned char grp1Buf[sizeof(Group)];
         Group *g1{static_cast<Group *>(static_cast<void *>(grp1Buf))};

         unsigned char grp2Buf[sizeof(Group)];
         Group *g2{static_cast<Group *>(static_cast<void *>(grp2Buf))};

         bool empty = testGroupMoveCtor<Group, VectorMock<Shape *>>(g1, g2);
         EXPECT(empty);

         g2->~Group();
         // expect a crash here if the tests above failed and we still get here
         g1->~Group();
     }},
#endif // HAS_GROUP_MOVE_CTOR

#ifdef HAS_GROUP_MOVE_ASSIGNMENT
    {CASE("VectorMock move assignment must copy")
     {
     }},
    {CASE("Group move assignment must empty moved-from object")
     {
         unsigned char grp1Buf[sizeof(Group)];
         Group *g1{static_cast<Group *>(static_cast<void *>(grp1Buf))};

         unsigned char grp2Buf[sizeof(Group)];
         Group *g2{static_cast<Group *>(static_cast<void *>(grp2Buf))};

         bool empty = testGroupMoveAss<Group, VectorMock<Shape *>>(g1, g2);
         EXPECT(empty);

         g2->~Group();
         // expect a crash here if the tests above failed and we still get here
         g1->~Group();
     }},
#endif // HAS_GROUP_MOVE_ASSIGNMENT
};

const size_t groupSpecialTestNum = sizeof(groupSpecialTests) / sizeof(groupSpecialTests[0]);
} // namespace exerciseTest

int main(int argc, char *argv[])
{
    int status = 0;
    status += lest::run(lest::tests{exerciseTest::groupSpecialTests, exerciseTest::groupSpecialTests+exerciseTest::groupSpecialTestNum}, argc, argv, std::cerr);

    std::clog << status << " test suites failed.\n";

    return status;
}
