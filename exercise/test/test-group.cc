/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "allocmock.hh"

#include "lest.hpp"

#include <group.hh>
#include <text.hh>
#include <pathShape.hh>
#include <rectangle.hh>

#include <type_traits>
#include <typeinfo>

//#include <iostream>
//#include <cstring>

/*
 * testing group move
 *
 * Group constructor and destructor don't do any external
 * actions (except new/delete).
 * So we need to provide a mock derived from Group to track these.
 * This is fine as Group is designed for derivation.
 * This doesn't help.
 * What we need to check are operations on elements.
 * One option is to mock Shape for Group test only (providing a mock header).
 * Another option is to provide our own new/delete and track these.
 * Our new/delete provide mocks for trackNew/trackDelete.
 */

namespace
{
using namespace exercise;

struct CDtorWrap
{
    virtual void ctor() {}
    virtual void dtor() {}
};
CDtorWrap globalCDtor;

struct CDtorMock : public CDtorWrap
{
/*
    MAKE_MOCK0(ctor, void());
*/
    MAKE_MOCK0(dtor, void());
};

struct GroupMock : public Group
{
/*
    GroupMock()
      : Group()
    {
        cdWrap->ctor();
    }
*/
    GroupMock() = default;
    GroupMock(CDtorWrap *w)
      : Group{}
      , cdWrap{w}
    {
    }

    // generated move is fine as long as Group move is fine
    // but it's not generated because we have our own dtor
    GroupMock(GroupMock &&) = default;

    ~GroupMock()
    {
        cdWrap->dtor();
    }

    GroupMock &operator=(GroupMock &&) = default;

    bool checkPosition(Position other) const
    {
        return xy.x == other.x && xy.y == other.y;
    }

    CDtorWrap *cdWrap{&globalCDtor};
};

#ifdef HAVE_PATH_SHAPE_TEMPL
#ifdef HAVE_PATH_SHAPE_TEMPL_BOOL
#define FRAME_INIT new PathShape<Rectangle, false>{Rectangle{70, 30}, "F1"}
typedef PathShape<Rectangle, false> *FrameType;
#else
typedef PathShape<Rectangle> *FrameType;
#define FRAME_INIT new PathShape<Rectangle>{Rectangle{70, 30}, "F1", false}
#endif // HAVE_PATH_SHAPE_TEMPL_BOOL
#else
typedef PathShape *FrameType;
#define FRAME_INIT new PathShape{new Rectangle{70, 30}, "F1", false}
#endif // HAVE_PATH_SHAPE_TEMPL

struct TestGroup
{
    TestGroup()
      : txt1{new Text{"Hello", "T1", Position{20, 20}}}
      , frame{FRAME_INIT}
      , txt2{new Text{"World!", "T2", Position{150, 70}}}
      , g{"LongNameToAvoidSSO"}
    {
        g.addChild(txt1);
        g.addChild(frame);
        g.move(10, 10);
    }

    ~TestGroup()
    {
        delete txt2;
        // frame and txt1 are cleaned up by g's dtor
    }

    Text *txt1;
    FrameType frame;
    Text *txt2;
    Group g;
};
} // unnamed namespace

#if 0 // this check doesn't work for now
static_assert(!std::is_copy_constructible<Group>::value, "Group must not be copy constructible");
static_assert(!std::is_copy_assignable<Group>::value, "Group must not be copy assignable");
#endif

#ifdef HAS_GROUP_MOVE_CTOR
static_assert(std::is_move_constructible<Group>::value, "Group must be move constructible");
#endif

#ifdef HAS_GROUP_MOVE_ASSIGNMENT
static_assert(std::is_move_assignable<Group>::value, "Group must be move assignable");
#endif

// /* GCC 5.1 has a bug here... */
// /**/


namespace exerciseTest
{
using namespace exercise;

bool testOk();

using trompeloeil::_;

extern const lest::test groupTests[] =
{
#ifdef _WIN32
    {CASE("Empty test (to keep VC++ happy)")
     {
         EXPECT(testOk());
     }},
#else
    {CASE("Group ctor/dtor memory calls must match")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management
             {
                 ALLOW_CALL(tracer, trackNew(_, _));
                 ALLOW_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 TestGroup gTest;
             }
             EXPECT(testOk());
             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},
#endif
#ifdef HAS_GROUP_MOVE_CTOR
#ifndef _WIN32
    {CASE("Group move ctor must not cause any memory calls")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management

             TestGroup gTest;

             Group g2;
             g2.~Group();

             {
                 FORBID_CALL(tracer, trackNew(_, _));
                 FORBID_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 new (&g2) Group{std::move(gTest.g)};
             }
             EXPECT(testOk());
             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},

    {CASE("Group move ctor must move ownership (no destruction of moved from)")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         {
             TestGroup gTest;

             Group g2;
             g2.~Group();

             {
                 FORBID_CALL(tracer, trackNew(_, _)); // !!! doesn't work w/ apply in ~Group
                 FORBID_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 new (&g2) Group{std::move(gTest.g)};
                 gTest.g.~Group(); // apply() may cause new!!!
             }
             EXPECT(testOk());

             new (&gTest.g) Group{}; // may cause new
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},
#endif

    {CASE("Group move ctor must move parent")
     {
         Position testPos{10, -44.33};
         GroupMock gTest;
         gTest.setPosition(testPos);

         GroupMock g2;
         g2.~GroupMock();

         new (&g2) Group{std::move(gTest)};

         EXPECT(g2.checkPosition(testPos));
     }},
#endif // HAS_GROUP_MOVE_CTOR

#ifdef HAS_GROUP_MOVE_ASSIGNMENT
    {CASE("Group move assignment must not use dtor") // as we can't use ctor
     {
         CDtorMock cdMock;
         int dtorCnt{0};
         GroupMock gTest;

         GroupMock g2{&cdMock};
         {
             // trompeloeil throws in FORBID_CALL, which isn't good in dtor
             //FORBID_CALL(cdMock, dtor());
             ALLOW_CALL(cdMock, dtor())
                 .LR_SIDE_EFFECT(++dtorCnt);

             g2 = std::move(gTest);
         }
         EXPECT(testOk());
         EXPECT(dtorCnt == 0);
     }},

    {CASE("Group move assignment must not use ctor")
     {
         GroupMock gTest;

         GroupMock g2;
         Group *p = &g2;

         g2 = std::move(gTest);

         EXPECT(typeid(*p) == typeid(GroupMock));
     }},

    {CASE("Group move assignment must move parent")
     {
         Position testPos{10, -44.33};
         GroupMock gTest;
         gTest.setPosition(testPos);

         GroupMock g2;

         g2 = std::move(gTest);

         EXPECT(g2.checkPosition(testPos));
     }},

#ifndef _WIN32
    {CASE("Group move assignment w/ empty target must not cause any memory calls")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         {
             TestGroup gTest;

             Group g2;
             g2.~Group();

             // default ctor may cause new
             {
                 ALLOW_CALL(tracer, trackNew(_, _));
                 FORBID_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 new (&g2) Group{};
             }
             {
                 FORBID_CALL(tracer, trackNew(_, _));
                 ALLOW_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 g2 = std::move(gTest.g);
             }
             EXPECT(testOk());
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},

    {CASE("Group move assignment must release old target")
     {
         AllocMock testAlloc;
         int gTestMem = 0;
         {
             TestGroup gTest;

             {
                 gTestMem = testAlloc.getCount();
                 Group g2;
                 Text *txt(new Text{"Test2", "T3", Position{150, 70}});
                 g2.addChild(txt);

                 g2 = std::move(gTest.g); // here old g2 should get cleaned up

                 EXPECT(testAlloc.getCount() == gTestMem);
             } // here new g2 (old gTest.g) should get cleaned up
               // we don't know exactly the count of gTest.txt2 allocs
             EXPECT(testAlloc.getCount() < gTestMem);
             EXPECT(testAlloc.getCount() > 0);
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},

    {CASE("Group move assignment must move ownership (no destruction of moved from)")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         {
             TestGroup gTest;
             Group g2;
             g2.~Group();

             // default ctor may cause new
             {
                 ALLOW_CALL(tracer, trackNew(_, _));
                 FORBID_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 new (&g2) Group{};
             }
             {
                 FORBID_CALL(tracer, trackNew(_, _));
                 ALLOW_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 g2 = std::move(gTest.g);
             }
             {
                 FORBID_CALL(tracer, trackNew(_, _));
                 FORBID_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 gTest.g.~Group();
             }
             {
                 ALLOW_CALL(tracer, trackNew(_, _));
                 FORBID_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 new (&gTest.g) Group{};
             }
         }
         EXPECT(testOk());
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},
#endif
#endif // HAS_GROUP_MOVE_ASSIGNMENT
#ifdef HAS_GROUP_GET_SHAPE
    {CASE("Check Group::getShape() for moved children")
     {
         Group g{"g"};
         auto t1 = new Text{"Hello", "Text1"};
         auto t2 = new Text{"World", "Text2"};

         g.addChild(t1);
         EXPECT(g.getShape("Text1").get() == t1);
         EXPECT(g.getShape("Text2").get() == nullptr);

         *t1 = std::move(*t2);
         EXPECT(g.getShape("Text2").get() == t1);
         EXPECT(g.getShape("Text1").get() == nullptr);
     }},
#endif // HAS_GROUP_GET_SHAPE
};
extern const size_t groupTestNum = sizeof(groupTests) / sizeof(groupTests[0]);
} // namespace exerciseTest
