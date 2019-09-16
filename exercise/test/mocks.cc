// mock objects needed in a separate translation unit
/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "allocmock.hh"
#include "cairomock.hh"
#ifdef _WIN32
#include "w32mock.hh"
#include "w32win-mock.hh"
#else
#include "xmock.hh"
#include "xwin-mock.hh"
#include <new>
#endif

namespace
{
#ifndef _WIN32
typedef std::vector<exerciseTest::AllocMock *, exerciseTest::StackAllocator<exerciseTest::AllocMock *, 10>> AllocVec;
#endif
} // unnamed namespace


inline namespace graphMock
{
#ifdef _WIN32
int RegisterClass(WNDCLASS *wc)
{
    return exerciseTest::W32Wrap::getCurrent().RegisterClass(wc);
}
void UnregisterClass(char const *nm, int mod)
{
    return exerciseTest::W32Wrap::getCurrent().UnregisterClass(nm, mod);
}
HWND CreateWindow(char const *clNm, char const *wNm,
                 UINT style, int x, int y, int w, int h,
                 int *p1, int *p2, int mod, int *p3)
{
    return exerciseTest::W32Wrap::getCurrent()
               .CreateWindow(clNm, wNm, style, x, y, w, h, p1, p2, mod, p3);
}
void DestroyWindow(HWND w)
{
    return exerciseTest::W32Wrap::getCurrent().DestroyWindow(w);
}
HDC GetDC(HWND w)
{
    return exerciseTest::W32Wrap::getCurrent().GetDC(w);
}
void ReleaseDC(HWND w, HDC dc)
{
    return exerciseTest::W32Wrap::getCurrent().ReleaseDC(w, dc);
}
#else
Display *XOpenDisplay(char const *name)
{
    return exerciseTest::XWrap::getCurrent().XOpenDisplay(name);
}
int XCloseDisplay(Display *dpy)
{
    return exerciseTest::XWrap::getCurrent().XCloseDisplay(dpy);
}

Window XCreateSimpleWindow(
    Display *dpy
    , Window rootW
    , int x, int y
    , unsigned int w, unsigned int h
    , unsigned int bw
    , unsigned long bcol, unsigned long bgcol)
{
    return exerciseTest::XWrap::getCurrent().XCreateSimpleWindow(dpy, rootW, x, y, w, h, bw, bcol, bgcol);
}
int XDestroyWindow(Display *dpy, Window w)
{
    return exerciseTest::XWrap::getCurrent().XDestroyWindow(dpy, w);
}

int XNextEvent(Display *dpy, XEvent *ev)
{
    return exerciseTest::XWrap::getCurrent().XNextEvent(dpy, ev);
}

int XMaskEvent(Display *, long, XEvent *)
{
    return 1;
}
#endif


cairo_surface_t *cairo_xlib_surface_create(
    Display *dpy
    , Drawable d
    , Visual *v
    , int w
    , int h)
{
    return exerciseTest::CairoWrap::getCurrent().cairo_xlib_surface_create(dpy, d, v, w, h);
}

cairo_surface_t *cairo_win32_surface_create(HDC w)
{
    return exerciseTest::CairoWrap::getCurrent().cairo_win32_surface_create(w);
}
void cairo_surface_destroy(cairo_surface_t *s)
{
    return exerciseTest::CairoWrap::getCurrent().cairo_surface_destroy(s);
}

cairo_t *cairo_create(cairo_surface_t *s)
{
    return exerciseTest::CairoWrap::getCurrent().cairo_create(s);
}
void cairo_destroy(cairo_t *cr)
{
    return exerciseTest::CairoWrap::getCurrent().cairo_destroy(cr);
}
void cairo_show_text(cairo_t *cr, char const *txt)
{
    return exerciseTest::CairoWrap::getCurrent().cairo_show_text(cr, txt);
}

} // namespace graphMock


namespace exerciseTest
{
GuiWinMock *GuiWinWrap::mock = nullptr;

GuiWrap GuiWrap::defaultObj;
GuiWrap *GuiWrap::curObj = &defaultObj;

CairoWrap CairoWrap::defaultObj;
CairoWrap *CairoWrap::curObj = &defaultObj;

#ifndef _WIN32
//AllocTracer AllocTracer::globalObj;

AllocVec AllocMock::allocs;
// just a dummy to set vector's capacity
int sizedAllocMockAllocs{AllocMock::reserveAllocs()};

// we need always at least one alloc
AllocMock globalTopAlloc;
#endif
} // namespace exerciseTest

#ifndef _WIN32
void *operator new(size_t n)
{
    return exerciseTest::AllocMock::getCurrent().alloc(n);
}

void operator delete(void *p) noexcept
{
    exerciseTest::AllocMock::getCurrent().dealloc(p);
}

// for sized deallocation
void operator delete(void *p, long unsigned) noexcept
{
    exerciseTest::AllocMock::getCurrent().dealloc(p);
}
#endif
