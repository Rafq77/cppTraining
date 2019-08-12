/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "xmock.hh"
#include "lest.hpp"

#include <xwin.hh>

#include <type_traits>

namespace exerciseTest
{
using namespace exercise;

bool testOk();

using trompeloeil::_;

//#ifndef __clang__
// clang has a problem with this
// as long as we don't have constexpr if, we have no way to portably
// just test default creation and destruction
// this is because we have two pretty different interfaces
// for destroying the default display: via detach until exercise 3,
// via removing last shared pointer in exercise 4
#if 0
// we change the interface of XDisplay in exercise 4
// so it's a bit tricky to have a test that covers both versions
// for before exercise 4, we assume our interface from base
// for exercise 4 (and later) we assume that XDisplay::getDefault
// returns a resource holder that cleans up after itself (not a raw pointer)

template <typename T>
typename std::enable_if<std::is_pointer<T>::value>::type
testDefaultDisplay()
{
    XWin *w{nullptr};
    XDisplay d{w};
}
template <typename T>
typename std::enable_if<!std::is_pointer<T>::value>::type
testDefaultDisplay()
{
    XDisplay::getDefault();
}
#endif
//#endif //__clang__
extern const lest::test xwinTests[] =
{
    {CASE("xwin: create and destroy functions match")
     {
         //trompeloeil::stream_tracer log(std::cout);
         XMock testX;
         Display d;
         Display *dpy = &d;

         trompeloeil::sequence seq;

         {
             REQUIRE_CALL(testX, XOpenDisplay(_))
                 .RETURN(dpy)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _))
                 .WITH(_1 == dpy)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XDestroyWindow(_, _))
                 .WITH(_1 == dpy)
                 .WITH(_2 == 1)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCloseDisplay(_))
                 .WITH(_1 == dpy)
                 .RETURN(0)
                 .IN_SEQUENCE(seq);

             XWin win(1, 1, "t");
             // we don't test loop here as registerCallback has
             // two arguments in the first two versions
         }
         EXPECT(testOk());
     }},
    {CASE("xwin: check that we get a new default")
     {
         // XDisplay doesn't have a stable interface (see above)
         // so we just repeat the sequence above
         //trompeloeil::stream_tracer log(std::cout);
         XMock testX;
         Display d;
         Display *dpy = &d;

         trompeloeil::sequence seq;

         {
             REQUIRE_CALL(testX, XOpenDisplay(_))
                 .RETURN(dpy)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _))
                 .WITH(_1 == dpy)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XDestroyWindow(_, _))
                 .WITH(_1 == dpy)
                 .WITH(_2 == 1)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCloseDisplay(_))
                 .WITH(_1 == dpy)
                 .RETURN(0)
                 .IN_SEQUENCE(seq);

             //testDefaultDisplay<decltype(XDisplay::getDefault())>();
             XWin win(1, 1, "t");
         }
         EXPECT(testOk());
     }},
#ifdef HAS_WIN_MOVE_CTOR
    {CASE("Check XWin move ctor sequence")
     {
         //trompeloeil::stream_tracer log(std::cout);
         unsigned char wbuf1[sizeof(XWin)];
         XWin *w1{static_cast<XWin *>(static_cast<void *>(wbuf1))};

         unsigned char wbuf2[sizeof(XWin)];
         XWin *w2{static_cast<XWin *>(static_cast<void *>(wbuf2))};

         XMock testX;
         Display d;
         Display *dpy = &d;
         int wi1{0};
         int wi2{0};

         trompeloeil::sequence seq;

         {
             REQUIRE_CALL(testX, XOpenDisplay(_))
                 .RETURN(dpy)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _))
                 .WITH(_1 == dpy)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             new (w1) XWin{2, -2, "W1"};
             w1->registerCallback([&wi1] { ++wi1; });
             testX.addEvent(XEvent{Expose, { 0, 1 } });
         }
         EXPECT(testOk());
         w1->loop();
         EXPECT(wi1 == 1);

         {
             FORBID_CALL(testX, XOpenDisplay(_));
             FORBID_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _));

             new (w2) XWin{std::move(*w1)};
             w2->registerCallback([&wi2] { ++wi2; });
         }
         EXPECT(testOk());
         w2->loop(); // if this crashes, we have a problem with re-registering
         EXPECT(wi1 == 1);
         EXPECT(wi2 == 1);

         {
             FORBID_CALL(testX, XDestroyWindow(_, _));
             FORBID_CALL(testX, XCloseDisplay(_));

             w1->~XWin();
         }
         EXPECT(testOk());
         memset(wbuf1, 0, sizeof(XWin));
         w2->loop(); // if this crashes, we have a problem with re-registering
         EXPECT(wi1 == 1);
         EXPECT(wi2 == 2);

         {
             REQUIRE_CALL(testX, XDestroyWindow(_, _))
                 .WITH(_1 == dpy)
                 .WITH(_2 == 1)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCloseDisplay(_))
                 .WITH(_1 == dpy)
                 .RETURN(0)
                 .IN_SEQUENCE(seq);

             w2->~XWin();
         }
         EXPECT(testOk());
     }},
#endif // HAS_WIN_MOVE_CTOR
#ifdef HAS_WIN_MOVE_ASSIGNMENT
    {CASE("Check XWin move assignment sequence")
     {
         //trompeloeil::stream_tracer log(std::cout);
         unsigned char wbuf1[sizeof(XWin)];
         XWin *w1{static_cast<XWin *>(static_cast<void *>(wbuf1))};

         unsigned char wbuf2[sizeof(XWin)];
         XWin *w2{static_cast<XWin *>(static_cast<void *>(wbuf2))};

         XMock testX;
         Display d;
         Display *dpy = &d;
         int wi1{0};
         int wi2{0};

         trompeloeil::sequence seq;

         {
             REQUIRE_CALL(testX, XOpenDisplay(_))
                 .RETURN(dpy)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _))
                 .WITH(_1 == dpy)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             new (w1) XWin{99, -99, "W1"};
             w1->registerCallback([&wi1] { ++wi1; });
             testX.addEvent(XEvent{Expose, { 0, 1 } });
         }
         EXPECT(testOk());
         w1->loop();
         EXPECT(wi1 == 1);

         {
             FORBID_CALL(testX, XOpenDisplay(_));

             REQUIRE_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _))
                 .WITH(_1 == dpy)
                 .RETURN(2)
                 .IN_SEQUENCE(seq);

             new (w2) XWin{18, -18, "W2"};
             w2->registerCallback([&wi2] { ++wi2; });
             testX.addEvent(XEvent{Expose, { 0, 2 } });
         }
         EXPECT(testOk());
         w1->loop();
         EXPECT(wi1 == 2);
         EXPECT(wi2 == 1);

         {
             FORBID_CALL(testX, XOpenDisplay(_));
             FORBID_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _));
             FORBID_CALL(testX, XCloseDisplay(_));

             REQUIRE_CALL(testX, XDestroyWindow(_, _))
                 .WITH(_1 == dpy)
                 .WITH(_2 == 2)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             *w2 = std::move(*w1);
             testX.delEvent(XEvent{Expose, { 0, 2 } });
         }
         EXPECT(testOk());
         w2->loop();
         EXPECT(wi1 == 3);
         EXPECT(wi2 == 1);

         {
             FORBID_CALL(testX, XDestroyWindow(_, _));
             FORBID_CALL(testX, XCloseDisplay(_));

             w1->~XWin();
         }
         EXPECT(testOk());
         memset(wbuf1, 0, sizeof(XWin));
         w2->loop();
         EXPECT(wi1 == 4);
         EXPECT(wi2 == 1);

#ifndef _LIBCPP_ABI_VERSION
         // libc++ doesn't support move-self-assignment for unique_ptr
         // check self assignment
         {
             FORBID_CALL(testX, XOpenDisplay(_));
             FORBID_CALL(testX, XCreateSimpleWindow(_, _, _, _, _, _, _, _, _));
             FORBID_CALL(testX, XDestroyWindow(_, _));
             FORBID_CALL(testX, XCloseDisplay(_));

             *w2 = std::move(*w2);
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(testX, XDestroyWindow(_, _))
                 .WITH(_1 == dpy)
                 .WITH(_2 == 1)
                 .RETURN(1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testX, XCloseDisplay(_))
                 .WITH(_1 == dpy)
                 .RETURN(0)
                 .IN_SEQUENCE(seq);

             w2->~XWin();
         }
         EXPECT(testOk());
#endif // _LIBCPP_ABI_VERSION
     }},
#endif // HAS_WIN_MOVE_ASSIGNMENT
};
extern const size_t xwinTestNum = sizeof(xwinTests) / sizeof(xwinTests[0]);
} // namespace exerciseTest
