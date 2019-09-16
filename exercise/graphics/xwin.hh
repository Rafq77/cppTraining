/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef XWIN_HH_SEEN_
#define XWIN_HH_SEEN_
#include "extgraph.hh"
#include <string>
#include <vector>
#include <functional>

namespace exercise
{
class XWin;

class XDisplayHolder
{
public:
    XDisplayHolder();
    ~XDisplayHolder();

    void attach(XWin *);
    bool detach(XWin *);

    Display *get() { return dpy; }
    Display *get() const { return dpy; }

    void loop() const;
    void waitExpose(Window w) const;

private:
    XDisplayHolder(XDisplayHolder const &);
    XDisplayHolder& operator=(XDisplayHolder const &);

    bool handleEvent(XEvent &e) const;

    Display *dpy;
    int count;
    std::vector<XWin *> winList;
};

class XDisplay
{
public:
    XDisplay(XWin *);
    ~XDisplay();

    XDisplayHolder *operator->();

    Display *cPtr() { return d->get(); }
    Display *cPtr() const { return d->get(); }

    void loop() const { d->loop(); }

    static XDisplayHolder *getDefault();

private:
    XDisplay(XDisplay const &);
    XDisplay& operator=(XDisplay const &);

    static XDisplayHolder *defDisplay;

    XDisplayHolder *d;
    XWin *win;
};

// for the callback we need type erasure
class Redraw
{
public:
    virtual ~Redraw();
    virtual void operator()() = 0;
};

class XWin
{
public:
    XWin(int width, int height, std::string const &name);
    ~XWin();

    cairo_surface_t *getSurface();
    void show() const;
    void redraw() const;
    void loop() const;

    void registerCallback(std::function<void()> fn); 
    void unregisterCallback();

    bool compare(Window other) const;

    std::string const &getName() const;

private:
    XWin(XWin const &);
    XWin& operator=(XWin const &);

    std::string name;
    XDisplay dpy;
    int scr;
    Window rootWin;
    int width;
    int height;
    Window win;
	std::function<void()> redrawF = nullptr;
};

typedef XWin GuiWin;

} // namespace exercise
#endif /* XWIN_HH_SEEN_ */
