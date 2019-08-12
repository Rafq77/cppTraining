/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CAIROMOCK_HH_SEEN_
#define CAIROMOCK_HH_SEEN_
#include "trompeloeil.hpp"
#include "graphmock.hh"

namespace exerciseTest
{
class CairoWrap
{
public:
    CairoWrap()
    {
        curObj = this;
    }
    virtual ~CairoWrap()
    {
        curObj = &defaultObj;
    }

    virtual cairo_surface_t *cairo_xlib_surface_create(
        Display *
        , Drawable
        , Visual *
        , int /*width*/
        , int /*height*/)
    {
        return &s;
    }
    virtual cairo_surface_t *cairo_win32_surface_create(HDC)
    {
        return &s;
    }
    virtual void cairo_surface_destroy(cairo_surface_t *)
    {
    }

    virtual cairo_t *cairo_create(cairo_surface_t *)
    {
        return &cr;
    }
    virtual void cairo_destroy(cairo_t *)
    {
    }
    virtual void cairo_show_text(cairo_t *, char const *)
    {
    }

    static CairoWrap &getCurrent()
    {
        return *curObj;
    }

private:
    static CairoWrap defaultObj;
    static CairoWrap *curObj;

    cairo_surface_t s;
    cairo_t cr;
};

struct CairoMock : public CairoWrap
{
    MAKE_MOCK5(cairo_xlib_surface_create, cairo_surface_t*(
                   Display *, Drawable, Visual *
                   , int, int));
    MAKE_MOCK1(cairo_win32_surface_create, cairo_surface_t *(HDC));
    MAKE_MOCK1(cairo_surface_destroy, void(cairo_surface_t *));
    MAKE_MOCK1(cairo_create, cairo_t*(cairo_surface_t *));
    MAKE_MOCK1(cairo_destroy, void(cairo_t *));
    MAKE_MOCK2(cairo_show_text, void(cairo_t *, char const *));
};
} // namespace exerciseTest
#endif /* CAIROMOCK_HH_SEEN_ */
