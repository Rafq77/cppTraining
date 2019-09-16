/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
// we use the same include guard as the real file
#ifndef XWIN_HH_SEEN_
#define XWIN_HH_SEEN_
#include "trompeloeil.hpp"
#include "graphmock.hh"

#include <functional>

namespace exerciseTest
{
class XWinWrap;

class XWinMock
{
public:
    XWinMock();
    ~XWinMock();

    MAKE_MOCK1(create, void(XWinWrap const *));
    MAKE_MOCK1(clean, void(XWinWrap const *));
    MAKE_MOCK0(getSurface, cairo_surface_t *());
    MAKE_MOCK2(registerCb, void(XWinWrap const *, std::function<void()>));
    MAKE_MOCK1(unregisterCb, void(XWinWrap const *));

private:
    XWinMock *oldMock = nullptr;
};
typedef XWinMock GuiWinMock;

class XWinWrap
{
public:
    XWinWrap(int, int, std::string const &)
    {
        if (mock) mock->create(this);
    }
    // I shouldn't get an implicit move (w/o the explicit),
    // but I get a copy, which is used for move
    XWinWrap(XWinWrap &&rhs)
      : valid{rhs.valid}
    {
        rhs.valid = false;
    }
    ~XWinWrap()
    {
        if (mock && valid) mock->clean(this);
    }
    XWinWrap &operator=(XWinWrap &&rhs)
    {
        if (this == &rhs) return *this;

        if (mock && valid) mock->clean(this);
        valid = rhs.valid;
        rhs.valid = false;

        return *this;
    }

    cairo_surface_t *getSurface()
    {
        static cairo_surface_t sfc{0};

        if (mock)
        {
            return mock->getSurface();
        } else {
            return &sfc;
        }
    }
    void show() const {}
    void destroy() {}
    void loop() const {}

    template <class FuncT, class ArgT>
    void registerCallback(FuncT f, ArgT a)
    {
        if (mock) mock->registerCb(this, [f,a] { f(a); });
    }
    void registerCallback(std::function<void()> f)
    {
        if (mock) mock->registerCb(this, f);
    }
    void unregisterCallback()
    {
        if (mock) mock->unregisterCb(this);
    }

    static XWinMock *setMock(XWinMock *m)
    {
        XWinMock *oldMock = mock;
        mock = m;

        return oldMock;
    }

    static void unsetMock(XWinMock *old)
    {
        mock = old;
    }

private:
    static XWinMock *mock;
    bool valid{true};
};
typedef XWinWrap GuiWinWrap;

inline XWinMock::XWinMock()
{
    oldMock = XWinWrap::setMock(this);
}
inline XWinMock::~XWinMock()
{
    XWinWrap::unsetMock(oldMock);
}

} // namespace exerciseTest

namespace exercise
{
typedef exerciseTest::XWinWrap GuiWin;
} // namespace exercise
#endif /* XWIN_HH_SEEN_ */
