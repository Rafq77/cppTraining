/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "xwin.hh"

#include "extgraph.hh"

#include <stdexcept>
#include <algorithm>
#include <functional>

namespace exercise
{
XDisplayHolder::XDisplayHolder()
  : dpy(XOpenDisplay(0))
  , count(0)
{
    if (!dpy)
    {
        throw std::runtime_error("No Display");
    }
}

XDisplayHolder::~XDisplayHolder()
{
#ifdef DEBUG
    if (count != 0)
    {
        throw std::runtime_error("Still somebody connected");
    }
#endif
}

void XDisplayHolder::attach(XWin *w)
{
    ++count;
    winList.push_back(w);
}

bool XDisplayHolder::detach(XWin *w)
{
    std::vector<XWin *>::iterator del
        = std::find(winList.begin(), winList.end(), w);
    if (del == winList.end())
    {
        throw std::runtime_error{"Trying to detach non-existing window"};
    }
    winList.erase(del);

    --count;
    if (count == 0)
    {
        XCloseDisplay(dpy);
        return true;
    }

    return false;
}

void XDisplayHolder::loop() const
{
    XEvent e;
    do
    {
        XNextEvent(dpy, &e);
    } while (handleEvent(e));
}

// wait for an expose event for win, but don't handle it
void XDisplayHolder::waitExpose(Window w) const
{
    XEvent e;
    while (XMaskEvent(dpy, ExposureMask, &e) == 0)
    {
        if (e.xexpose.window == w)
        {
            break;
        }

        handleEvent(e);
    }
}

bool XDisplayHolder::handleEvent(XEvent &e) const
{
    if (e.type==Expose && e.xexpose.count<1)
    {
		auto w = std::find_if(winList.begin(), winList.end(), [win=e.xexpose.window](XWin* x) {
				return x->compare(win);
		});

        if (w != winList.end())
        {
            (*w)->redraw();
        }
        else
        {
            // it can happen that we get events even after the
            // window was removed
            // so we just ignore a 'not found'
        }
    }
    else if (e.type==ButtonPress || e.type==ClientMessage)
    {
        return false;
    }
    return true;
}

XDisplay::XDisplay(XWin *w)
  : d(getDefault())
  , win(w)
{
    d->attach(win);
}

XDisplay::~XDisplay()
{
    if (d->detach(win) && d == defDisplay)
    {
        defDisplay = 0;
    }
}

XDisplayHolder *XDisplay::operator->()
{
    return d;
}

XDisplayHolder *XDisplay::getDefault()
{
    if (!defDisplay)
    {
        defDisplay = new XDisplayHolder;
    }
    return defDisplay;
}

XDisplayHolder *XDisplay::defDisplay = 0;

Redraw::~Redraw()
{
}

XWin::XWin(int w, int h, std::string const &nm)
  : name(nm)
  , dpy(this)
  , scr(DefaultScreen(dpy.cPtr()))
  , rootWin(RootWindow(dpy.cPtr(), scr))
  , width(w)
  , height(h)
  , win(XCreateSimpleWindow(dpy.cPtr(), rootWin
                            , 1, 1, w, h
                            , 0, BlackPixel(dpy.cPtr(), scr)
                            , WhitePixel(dpy.cPtr(), scr)))
{
    if (!win)
    {
        throw std::runtime_error("No Window");
    }
    XStoreName(dpy.cPtr(), win, name.c_str());
    Atom delWindow[] = { XInternAtom(dpy.cPtr(), "WM_DELETE_WINDOW", 0) };
    XSetWMProtocols(dpy.cPtr(), win, delWindow, 1);
    XSelectInput(dpy.cPtr(), win, ExposureMask|ButtonPressMask);
    // wait for Expose event before drawing anything
    XMapWindow(dpy.cPtr(), win);
    dpy->waitExpose(win);
}

XWin::~XWin()
{
    XDestroyWindow(dpy.cPtr(), win);
}

cairo_surface_t *XWin::getSurface()
{
    return cairo_xlib_surface_create(dpy.cPtr(), win,
                                     DefaultVisual(dpy.cPtr(), 0),
                                     width, height);
}

void XWin::show() const
{
    XFlush(dpy.cPtr());
}

void XWin::redraw() const
{
	if (redrawF)
		redrawF();
}

void XWin::loop() const
{
    dpy.loop();
}

void XWin::registerCallback(std::function<void()> fn)
{ 
	redrawF = fn;
}

void XWin::unregisterCallback()
{
	//redrawF = [](){};
	redrawF = nullptr;
}

bool XWin::compare(Window other) const
{
    return other == win;
}

std::string const &XWin::getName() const
{
    return name;
}
} // namespace exercise
