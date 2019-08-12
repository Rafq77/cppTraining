/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "canvas.hh"

#include "pathShape.hh"
#include "rectangle.hh"
#include "text.hh"

#include "extgraph.hh"
#include <string>
#include <vector>
#include <functional>

namespace exercise
{
CanvasImpl::CanvasImpl(int width, int height, std::string const &name)
  : win{new GuiWin{width, height, name}}
  , surface{win->getSurface()}
  , cr{cairo_create(surface)}
{
    win->registerCallback([this] { draw(); });
    (*this) += new PathShape<Rectangle, true>(Rectangle(width, height), "Background", Position(0, 0), Color::White);
    show();
}

CanvasImpl::~CanvasImpl()
{
    win->unregisterCallback();
    for (Shape *p: elems)
    {
        delete p;
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    delete win;
}

void CanvasImpl::operator+=(Shape *s)
{
    elems.push_back(s);
}

void CanvasImpl::draw() const
{
    for (Shape const *p: elems)
    {
        p->draw(cr);
    }
    cairo_show_page(cr);
}

void CanvasImpl::show() const
{
    draw();
    win->show();
}

void CanvasImpl::startLoop()
{
    win->loop();
}


Canvas::Canvas(int width, int height, std::string const &name)
  : pImpl{new CanvasImpl{width, height, name}}
{
}

Canvas::~Canvas()
{
    delete pImpl;
}

void Canvas::operator+=(Shape *s)
{
    (*pImpl) += s;
}

void Canvas::draw() const
{
    pImpl->draw();
}

void Canvas::show() const
{
    pImpl->show();
}

void Canvas::startLoop()
{
    pImpl->startLoop();
}


} // namespace exercise
