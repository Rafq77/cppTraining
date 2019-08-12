/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "cairomock.hh"

#include "allocmock.hh"
#ifdef _WIN32
#include "w32win-mock.hh"
#else
#include "xwin-mock.hh"
#endif

#include "test-classes.hh"

#include "lest.hpp"

#include <canvas.hh>
#include <text.hh>

#include <type_traits>

namespace exerciseTest
{
using namespace exercise;

using trompeloeil::_;

static_assert(!std::is_copy_constructible<Canvas>::value, "Canvas must not be copy constructible");
static_assert(!std::is_copy_assignable<Canvas>::value, "Canvas must not be copy assignable");

#ifdef HAS_CANVAS_MOVE_CTOR
static_assert(std::is_move_constructible<Canvas>::value, "Canvas must be move constructible");
#endif
#ifdef HAS_CANVAS_MOVE_ASSIGNMENT
static_assert(std::is_move_assignable<Canvas>::value, "Canvas must be move assignable");
#endif

extern const lest::test canvasTests[] =
{
    {CASE("canvas: create and destroy functions match")
     {
         //trompeloeil::stream_tracer log(std::cerr);
         GuiWinMock testWin;

         CairoMock testCairo;
         cairo_t cr;
         cairo_t *crp = &cr;
         cairo_surface_t sf;
         cairo_surface_t *sfp = &sf;

         auto testShape = new ShapeMock;
         std::function<void()> cb{nullptr};

         trompeloeil::sequence seq;

         {
             REQUIRE_CALL(testWin, create(_))
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, getSurface())
                 .RETURN(sfp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_create(_))
                 .WITH(_1 == sfp)
                 .RETURN(crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_SIDE_EFFECT(cb = _2)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, unregisterCb(_))
                 .LR_SIDE_EFFECT(cb = nullptr;);

             REQUIRE_CALL(testCairo, cairo_destroy(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_surface_destroy(_))
                 .WITH(_1 == sfp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, clean(_))
                 .IN_SEQUENCE(seq);

             Canvas c{1, 44, "tc"};
             EXPECT(testOk());

             EXPECT(cb != nullptr);

             c += testShape;
             cb();
         }
         EXPECT(cb == nullptr);
         EXPECT(testOk());
     }},

#ifndef _WIN32
    {CASE("Canvas: check shape release in dtor")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc{};

         //trompeloeil::stream_tracer log(std::cout);

         {
             Text *txt1{nullptr};
             Text *txt2{nullptr};

             int cnt1{0};

/*
 * Intercepting new/delete is a bit complicated...
 * ALLOW_CALL creates objects using new (and releases them with delete).
 * trackNew() is only provided in tracer after the expectation is
 * setup with ALLOW_CALL.
 * So we can't use the tracer before trackNew() is setup.
 * As we setup the tracer only after setting up trackNew(), these new()-
 * calls are not counted.
 */
             {
                 ALLOW_CALL(tracer, trackDelete(_))
                     .LR_SIDE_EFFECT(--cnt1);

                 ALLOW_CALL(tracer, trackNew(_, _))
                     .LR_SIDE_EFFECT(++cnt1);

                 TraceSetter ts{testAlloc, &tracer};

                 txt1 = new Text{"Hello", "T1"};
                 txt2 = new Text{"C++", "T2"};
             }
             EXPECT(testOk());
             int cnt2{0};
             {
                 ALLOW_CALL(tracer, trackDelete(_))
                     .LR_SIDE_EFFECT(--cnt2);

                 ALLOW_CALL(tracer, trackNew(_, _))
                     .LR_SIDE_EFFECT(++cnt2);

                 TraceSetter ts{testAlloc, &tracer};

                 Canvas c(1, 44, "tc");

                 c += txt1;
                 c += txt2;
             }

             EXPECT(testOk());
             EXPECT(cnt2 == -cnt1);
         }

         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},
#endif

    {CASE("Canvas: check drawing order")
     {
         //trompeloeil::stream_tracer log(std::cout);
         Canvas c{2, 3, "c"};
         c += new Text{"1", "T1"};
         c += new Text{"2", "T2"};
         c += new Text{"3", "T3"};
         c += new Text{"4", "T4"};
         c += new Text{"5", "T5"};

         CairoMock testCairo;

         trompeloeil::sequence seq;

         {
             REQUIRE_CALL(testCairo, cairo_show_text(_, _))
                 .WITH(*_2 == '1')
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_show_text(_, _))
                 .WITH(*_2 == '2')
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_show_text(_, _))
                 .WITH(*_2 == '3')
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_show_text(_, _))
                 .WITH(*_2 == '4')
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_show_text(_, _))
                 .WITH(*_2 == '5')
                 .IN_SEQUENCE(seq);

             c.show();
         }

         EXPECT(testOk());
     }},

#ifdef HAS_CANVAS_MOVE_CTOR
    {CASE("Check Canvas move ctor sequence")
     {
         //trompeloeil::stream_tracer log(std::cout);
         unsigned char cbuf1[sizeof(Canvas)];
         Canvas *c1{static_cast<Canvas *>(static_cast<void *>(cbuf1))};

         unsigned char cbuf2[sizeof(Canvas)];
         Canvas *c2{static_cast<Canvas *>(static_cast<void *>(cbuf2))};

         GuiWinMock testWin;
         GuiWinWrap const * win1{nullptr};
         GuiWinWrap const * win2{nullptr};

         CairoMock testCairo;
         cairo_t cr;
         cairo_t *crp = &cr;
         cairo_surface_t sf;
         cairo_surface_t *sfp = &sf;

         auto testShape = new ShapeMock;

         trompeloeil::sequence seq;
         std::function<void()> cb1{nullptr};
         std::function<void()> cb2{nullptr};

         {
             REQUIRE_CALL(testWin, create(_))
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, getSurface())
                 .RETURN(sfp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_create(_))
                 .WITH(_1 == sfp)
                 .RETURN(crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_SIDE_EFFECT(win1 = _1)
                 .LR_SIDE_EFFECT(cb1 = _2)
                 .IN_SEQUENCE(seq);

             new (c1) Canvas{77, -77, "C"};
             *c1 += testShape;
         }
         EXPECT(testOk());
         EXPECT(cb1 != nullptr);

         {
             FORBID_CALL(testWin, create(_));
             FORBID_CALL(testWin, getSurface());
             FORBID_CALL(testCairo, cairo_create(_));

             ALLOW_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win1)
                 .LR_SIDE_EFFECT(cb1 = nullptr)
                 .IN_SEQUENCE(seq);
             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_WITH(cb1 == nullptr)
                 .LR_SIDE_EFFECT(win2 = _1)
                 .LR_SIDE_EFFECT(cb2 = _2)
                 .IN_SEQUENCE(seq);
             ALLOW_CALL(*testShape, doDraw(_));

             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             new (c2) Canvas{std::move(*c1)};
             if (cb1) // we may have pimpl
             {
                 cb2 = cb1;
                 cb1 = nullptr;
                 win2 = win1;
             }
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp);

             cb2();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             cb2();
         }
         EXPECT(testOk());

         {
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             c1->~Canvas();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             cb2();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win2)
                 .LR_SIDE_EFFECT(cb2 = nullptr;)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_destroy(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_surface_destroy(_))
                 .WITH(_1 == sfp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, clean(_))
                 .IN_SEQUENCE(seq);

             c2->~Canvas();
         }
         EXPECT(testOk());
     }},

    // this is optional: we don't really require a moved-from
    // object to be able to move again
    {CASE("Check correct moved-from ctor handling for Canvas")
     {
         //trompeloeil::stream_tracer log(std::cout);
         unsigned char cbuf1[sizeof(Canvas)];
         Canvas *c1{static_cast<Canvas *>(static_cast<void *>(cbuf1))};

         unsigned char cbuf2[sizeof(Canvas)];
         Canvas *c2{static_cast<Canvas *>(static_cast<void *>(cbuf2))};

         unsigned char cbuf3[sizeof(Canvas)];
         Canvas *c3{static_cast<Canvas *>(static_cast<void *>(cbuf3))};

         GuiWinMock testWin;
         auto testShape = new ShapeMock;
         GuiWinWrap const * win1{nullptr};
         GuiWinWrap const * win2{nullptr};

         std::function<void()> cb1{nullptr};
         std::function<void()> cb2{nullptr};

         CairoMock testCairo;
         cairo_t cr;
         cairo_t *crp = &cr;
         cairo_surface_t sf;
         cairo_surface_t *sfp = &sf;

         // setup and allow everything (already tested before)
         {
             ALLOW_CALL(testWin, create(_));
             ALLOW_CALL(testWin, getSurface())
                 .RETURN(sfp);
             ALLOW_CALL(testCairo, cairo_create(_))
                 .RETURN(crp);
             REQUIRE_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_SIDE_EFFECT(win1 = _1)
                 .LR_SIDE_EFFECT(cb1 = _2);

             new (c1) Canvas{77, -77, "C"};
             *c1 += testShape;
         }
         EXPECT(testOk());

         {
             ALLOW_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win1)
                 .LR_SIDE_EFFECT(cb1 = nullptr);
             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_WITH(cb1 == nullptr)
                 .LR_SIDE_EFFECT(win2 = _1)
                 .LR_SIDE_EFFECT(cb2 = _2);

             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             new (c2) Canvas{std::move(*c1)};
             if (cb1) // we may have pimpl
             {
                 cb2 = cb1;
                 cb1 = nullptr;
                 win2 = win1;
             }
         }
         EXPECT(testOk());

         // move moved-from again
         // this possibly just crashes in Canvas ctor if handled wrongly
         {
             FORBID_CALL(testWin, registerCb(_, ANY(std::function<void()>)));
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             new (c3) Canvas{std::move(*c1)};
         }
         EXPECT(testOk());

         {
             FORBID_CALL(testWin, registerCb(_, ANY(std::function<void()>)));
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             c3->~Canvas();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp);

             cb2();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win2);
             REQUIRE_CALL(testCairo, cairo_destroy(_));
             REQUIRE_CALL(testCairo, cairo_surface_destroy(_));
             REQUIRE_CALL(testWin, clean(_));

             c2->~Canvas();
         }
         EXPECT(testOk());

         {
             FORBID_CALL(testWin, registerCb(_, ANY(std::function<void()>)));
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             c1->~Canvas();
         }
         EXPECT(testOk());
     }},

#ifndef _WIN32
    // memory calls are ok (for std::function in registerCallback)
    // but they must add up to zero
    {CASE("Canvas move ctor must not cause any memory overhead")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management

             Canvas c1{2, 55, "tc1"};
             // we don't have a default ctor for Canvas
             Canvas c2{7, 13, "tc2"};
             c2.~Canvas();

             auto memCnt = testAlloc.getCount();
             new (&c2) Canvas{std::move(c1)};
             EXPECT(memCnt == testAlloc.getCount());

             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},

    {CASE("Canvas move ctor must move ownership (no destruction of moved from)")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management

             Canvas c1{2, 55, "tc1"};
             // we don't have a default ctor for Canvas
             Canvas c2{7, 13, "tc2"};
             c2.~Canvas();

             auto memCnt = testAlloc.getCount();
             new (&c2) Canvas{std::move(c1)};
             c1.~Canvas();
             new (&c1) Canvas{std::move(c2)};
             EXPECT(memCnt == testAlloc.getCount());

             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},
#endif
#endif // HAS_CANVAS_MOVE_CTOR
#ifdef HAS_CANVAS_MOVE_ASSIGNMENT
    {CASE("Check Canvas move assignment sequence")
     {
         //trompeloeil::stream_tracer log(std::cout);
         unsigned char cbuf1[sizeof(Canvas)];
         Canvas *c1{static_cast<Canvas *>(static_cast<void *>(cbuf1))};

         unsigned char cbuf2[sizeof(Canvas)];
         Canvas *c2{static_cast<Canvas *>(static_cast<void *>(cbuf2))};

         GuiWinMock testWin;
         GuiWinWrap const * win1{nullptr};
         GuiWinWrap const * win2{nullptr};
         GuiWinWrap const * win3{nullptr};

         CairoMock testCairo;
         cairo_t cr;
         cairo_t *crp = &cr;
         cairo_surface_t sf;
         cairo_surface_t *sfp = &sf;

         auto testShape1 = new ShapeMock;
         auto testShape2 = new ShapeMock;
         std::function<void()> cb1{nullptr};
         std::function<void()> cb2{nullptr};
         std::function<void()> cb3{nullptr};

         trompeloeil::sequence seq, seq2;

         {
             REQUIRE_CALL(testWin, create(_))
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, getSurface())
                 .RETURN(sfp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_create(_))
                 .WITH(_1 == sfp)
                 .RETURN(crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_SIDE_EFFECT(win1 = _1)
                 .LR_SIDE_EFFECT(cb1 = _2)
                 .IN_SEQUENCE(seq);

             new (c1) Canvas{77, -77, "C1"};
             *c1 += testShape1;
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(testWin, create(_))
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, getSurface())
                 .RETURN(sfp+1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_create(_))
                 .WITH(_1 == sfp+1)
                 .RETURN(crp+1)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_SIDE_EFFECT(win2 = _1)
                 .LR_SIDE_EFFECT(cb2 = _2)
                 .IN_SEQUENCE(seq);

             new (c2) Canvas{8, -8, "C2"};
             *c2 += testShape2;
         }
         EXPECT(testOk());
         EXPECT(win2 != win1);

         {
             REQUIRE_CALL(*testShape1, doDraw(_))
                 .WITH(_1 == crp);

             cb1();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape2, doDraw(_))
                 .WITH(_1 == crp+1);

             cb2();
         }
         EXPECT(testOk());

         {
             bool win2r = true;

             FORBID_CALL(testWin, create(_));
             FORBID_CALL(testWin, getSurface());
             FORBID_CALL(testCairo, cairo_create(_));

             // I don't require a sequence here, as the rule-of-zero
             // approach does it in the wrong order.
             // and for cairo it's ok.
             REQUIRE_CALL(testCairo, cairo_destroy(_))
                 .WITH(_1 == crp+1);

             REQUIRE_CALL(testCairo, cairo_surface_destroy(_))
                 .WITH(_1 == sfp+1);

             // unregister the old c2
             // possibly repeatedly register and unregister the old c2
             // but we require below that it finishes unregistered
             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .WITH(_1 == win2)
                 .LR_WITH(win2r == false)
                 .LR_SIDE_EFFECT(win2r = true);

             REQUIRE_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win2)
                 .TIMES(AT_LEAST(1))
                 .LR_SIDE_EFFECT(cb2 = nullptr;)
                 .LR_SIDE_EFFECT(win2r = false)
                 .IN_SEQUENCE(seq, seq2);

             REQUIRE_CALL(testWin, clean(_))
                 .WITH(_1 == win2)
                 .IN_SEQUENCE(seq);

             // unregister the old c1 and re-register the new c1
             ALLOW_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win1)
                 .LR_SIDE_EFFECT(cb1 = nullptr;)
                 .IN_SEQUENCE(seq2);

             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_WITH(cb1 == nullptr)
                 .LR_SIDE_EFFECT(win3 = _1)
                 .LR_SIDE_EFFECT(cb3 = _2)
                 .IN_SEQUENCE(seq2);

             *c2 = std::move(*c1);
             EXPECT(win2r == false);

             if (cb1) // we may have pimpl
             {
                 cb3 = cb1;
                 cb1 = nullptr;
                 win3 = win1;
             }
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape1, doDraw(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             cb3();
         }
         EXPECT(testOk());

         {
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             c1->~Canvas();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape1, doDraw(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             cb3();
         }
         EXPECT(testOk());

         // check self assignment
         {
             // while we want self assignment, we don't require it's a no-op
             FORBID_CALL(testWin, create(_));
             FORBID_CALL(testWin, getSurface());
             FORBID_CALL(testCairo, cairo_create(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             ALLOW_CALL(testWin, unregisterCb(_));
             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)));

             *c2 = std::move(*c2);
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape1, doDraw(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             cb3();
         }
         EXPECT(testOk());


         {
             REQUIRE_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win3)
                 .LR_SIDE_EFFECT(cb3 = nullptr;)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_destroy(_))
                 .WITH(_1 == crp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testCairo, cairo_surface_destroy(_))
                 .WITH(_1 == sfp)
                 .IN_SEQUENCE(seq);

             REQUIRE_CALL(testWin, clean(_))
                 .IN_SEQUENCE(seq);

             c2->~Canvas();
         }

         EXPECT(testOk());
     }},

    // this test case is only partly required:
    // assignment into moved-from is generally required,
    // but moving a moved-from again is not!
    {CASE("Check correct moved-from assignment handling for Canvas")
     {
         //trompeloeil::stream_tracer log(std::cout);
         unsigned char cbuf1[sizeof(Canvas)];
         Canvas *c1{static_cast<Canvas *>(static_cast<void *>(cbuf1))};

         unsigned char cbuf2[sizeof(Canvas)];
         Canvas *c2{static_cast<Canvas *>(static_cast<void *>(cbuf2))};

         unsigned char cbuf3[sizeof(Canvas)];
         Canvas *c3{static_cast<Canvas *>(static_cast<void *>(cbuf3))};

         GuiWinMock testWin;
         auto testShape = new ShapeMock;
         GuiWinWrap const * win1{nullptr};
         GuiWinWrap const * win2{nullptr};
         GuiWinWrap const * win3{nullptr};

         std::function<void()> cb1{nullptr};
         std::function<void()> cb2{nullptr};
         std::function<void()> cb3{nullptr};

         CairoMock testCairo;
         cairo_t cr;
         cairo_t *crp = &cr;
         cairo_surface_t sf;
         cairo_surface_t *sfp = &sf;

         // setup and allow everything (already tested before)
         {
             ALLOW_CALL(testWin, create(_));
             ALLOW_CALL(testWin, getSurface())
                 .RETURN(sfp);
             ALLOW_CALL(testCairo, cairo_create(_))
                 .RETURN(crp);
             REQUIRE_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_SIDE_EFFECT(win1 = _1)
                 .LR_SIDE_EFFECT(cb1 = _2);

             new (c1) Canvas{77, -77, "C"};
             *c1 += testShape;
         }
         EXPECT(testOk());

         {
             ALLOW_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win1)
                 .LR_SIDE_EFFECT(cb1 = nullptr);
             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_WITH(cb1 == nullptr)
                 .LR_SIDE_EFFECT(win2 = _1)
                 .LR_SIDE_EFFECT(cb2 = _2);

             new (c2) Canvas{std::move(*c1)};
             if (cb1) // we may have pimpl
             {
                 cb2 = cb1;
                 cb1 = nullptr;
                 win2 = win1;
             }
         }
         EXPECT(testOk());

         // move moved-from again
         // this possibly just crashes in Canvas ctor if handled wrongly
         {
             FORBID_CALL(testWin, registerCb(_, ANY(std::function<void()>)));
             FORBID_CALL(testWin, unregisterCb(_));

             new (c3) Canvas{std::move(*c1)};
         }
         EXPECT(testOk());

         {
             FORBID_CALL(testWin, registerCb(_, ANY(std::function<void()>)));
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             c3->~Canvas();
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp);

             cb2();
         }
         EXPECT(testOk());

         // move valid obj into moved-from
         {
             ALLOW_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win2)
                 .LR_SIDE_EFFECT(cb2 = nullptr);
             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .LR_WITH(cb2 == nullptr)
                 .LR_SIDE_EFFECT(win3 = _1)
                 .LR_SIDE_EFFECT(cb3 = _2);

             *c1 = std::move(*c2);
             if (cb2) // we may have pimpl
             {
                 cb3 = cb2;
                 cb2 = nullptr;
                 win3 = win2;
             }
         }
         EXPECT(testOk());

         {
             REQUIRE_CALL(*testShape, doDraw(_))
                 .WITH(_1 == crp);

             cb3();
         }
         EXPECT(testOk());

         // move moved-from into valid obj
         {
             // we allow 'Canvas tmp(std::move(*this)'
             bool win3r = true;

             ALLOW_CALL(testWin, registerCb(_, ANY(std::function<void()>)))
                 .WITH(_1 == win3)
                 .LR_WITH(win3r == false)
                 .LR_SIDE_EFFECT(win3r = true);

             ALLOW_CALL(testWin, unregisterCb(_))
                 .WITH(_1 == win3)
                 .LR_WITH(win3r == true)
                 .LR_SIDE_EFFECT(win3r = false);

             REQUIRE_CALL(testCairo, cairo_destroy(_));
             REQUIRE_CALL(testCairo, cairo_surface_destroy(_));
             REQUIRE_CALL(testWin, clean(_));

             *c1 = std::move(*c2);
             EXPECT(win3r == false);
         }
         EXPECT(testOk());

         // move moved-from into moved-from
         {
             FORBID_CALL(testWin, unregisterCb(_));
             FORBID_CALL(testCairo, cairo_destroy(_));
             FORBID_CALL(testCairo, cairo_surface_destroy(_));
             FORBID_CALL(testWin, clean(_));

             *c1 = std::move(*c2);
         }
         EXPECT(testOk());

         c1->~Canvas();
         c2->~Canvas();
         EXPECT(testOk());
     }},

#ifndef _WIN32
    // memory calls are ok (for std::function in registerCallback)
    // but they must add up to zero
    {CASE("Canvas move assignment w/ empty target must not cause any memory overhead")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management

             Canvas c1{2, 55, "tc1"};
             // we don't have a default ctor for Canvas
             Canvas c2{7, 13, "tc2"};
             Canvas c3{std::move(c2)}; // now c2 is empty

             auto memCnt = testAlloc.getCount();
             c2 = std::move(c1);
             EXPECT(memCnt == testAlloc.getCount());

             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},

    {CASE("Canvas move assignment must release old target")
     {
         AllocMock testAlloc;
         int cTestMem = 0;
         {
             Canvas cTest{2, 55, "tc1"};

             {
                 cTestMem = testAlloc.getCount();
                 Canvas c2{7, 13, "tc2"};
                 Text *txt(new Text{"Test2", "T3", Position{150, 70}});
                 c2 += txt;

                 c2 = std::move(cTest); // here old c2 should get cleaned up

                 EXPECT(testAlloc.getCount() == cTestMem);
             } // here new c2 (old cTest) should get cleaned up
             EXPECT(testAlloc.getCount() == 0);
         }
         EXPECT(testAlloc.ok());
     }},

    {CASE("Canvas move assignment must move ownership (no destruction of moved from)")
     {
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management

             Canvas c1{2, 55, "tc1"};
             // we don't have a default ctor for Canvas
             Canvas c2{7, 13, "tc2"};
             Canvas c3{std::move(c2)}; // now c2 is empty

             auto memCnt = testAlloc.getCount();

             Canvas tmp{std::move(c2)};
             c2 = std::move(c1);
             c1 = std::move(tmp);
             tmp.~Canvas();
             new (&tmp) Canvas{std::move(c1)};

             EXPECT(memCnt == testAlloc.getCount());

             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
         EXPECT(testAlloc.getCount() == 0);
     }},
#endif
#endif // HAS_CANVAS_MOVE_ASSIGNMENT
#ifdef HAS_CANVAS_GET_SHAPE
    {CASE("Check Canvas getShape() for moved elements")
     {
         Canvas c{2, 23, "c"};
         auto t1 = new Text{"Hello", "Text1"};
         auto t2 = new Text{"World", "Text2"};

         c += t1;
         EXPECT(c.getShape("Text1").get() == t1);
         EXPECT(c.getShape("Text2").get() == nullptr);

         *t1 = std::move(*t2);
         EXPECT(c.getShape("Text2").get() == t1);
         EXPECT(c.getShape("Text1").get() == nullptr);
     }},
#endif // HAS_CANVAS_GET_SHAPE
};
extern const size_t canvasTestNum = sizeof(canvasTests) / sizeof(canvasTests[0]);
} // namespace exerciseTest
