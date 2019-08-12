/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef XMOCK_HH_SEEN_
#define XMOCK_HH_SEEN_
#include "trompeloeil.hpp"
#include "graphmock.hh"
#include <vector>
#include <algorithm>

namespace exerciseTest
{
class XWrap
{
public:
    XWrap()
    {
        curObj = this;
    }
    virtual ~XWrap()
    {
        curObj = &defaultObj;
    }

    virtual Display *XOpenDisplay(char const *)
    {
        return &dpy;
    }
    virtual int XCloseDisplay(Display *)
    {
        return 0;
    }

    virtual Window XCreateSimpleWindow(
        Display *
        , Window
        , int, int
        , unsigned int, unsigned int
        , unsigned int
        , unsigned long, unsigned long)
    {
        return 17;
    }
    virtual int XDestroyWindow(Display *, Window)
    {
        return 0;
    }
    virtual int XNextEvent(Display *, XEvent *ev)
    {
        if (loopIdx < loopList.size())
        {
            *ev = loopList[loopIdx];
            ++loopIdx;
        } else
        {
            *ev = XEvent{ButtonPress, { 0, 0 } };
            loopIdx = 0;
        }
        return 0;
    }
    void addEvent(XEvent ev)
    {
        loopList.push_back(ev);
    }
    void delEvent(XEvent ev)
    {
        loopList.erase(std::find(loopList.begin(), loopList.end(), ev));
    }

    static XWrap &getCurrent()
    {
        return *curObj;
    }

private:
    static XWrap defaultObj;
    static XWrap *curObj;

    Display dpy;
    size_t loopIdx{0};
    std::vector<XEvent> loopList;
};
typedef XWrap GuiWrap;


struct XMock : public XWrap
{
    MAKE_MOCK1(XOpenDisplay, Display*(char const *));
    MAKE_MOCK1(XCloseDisplay, int(Display *));

    MAKE_MOCK9(XCreateSimpleWindow, Window(
                   Display *, Window
                   , int, int, unsigned int, unsigned int
                   , unsigned int, unsigned long, unsigned long));
    MAKE_MOCK2(XDestroyWindow, int(Display *, Window));
};
typedef XMock GuiMock;
} // namespace exerciseTest
#endif /* XMOCK_HH_SEEN_ */
