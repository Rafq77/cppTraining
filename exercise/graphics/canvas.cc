/*
 * Copyright (c) 2014-2019 Detlef Vollmann, vollmann engineering gmbh
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
Canvas::Canvas(int width, int height, std::string const &name)
  : win(new GuiWin(width, height, name))
  , surface(win->getSurface())
  , cr(cairo_create(surface))
{
    //win->registerCallback([t=this]() { draw(); } );
    win->registerCallback([t=this]() { t->draw(); });
    
    (*this) += new PathShape(new Rectangle(width, height), "Background", true, Position(0, 0), Color::White);
    show();
}

Canvas::~Canvas()
{
    win->unregisterCallback(); // it's the contract
    for (std::vector<Shape *>::iterator i = elems.begin();
         i != elems.end();
         ++i)
    {
        delete *i;
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    delete win;
}

void Canvas::operator+=(Shape *s)
{
    elems.push_back(s);
}

void Canvas::draw() const
{
    for (std::vector<Shape *>::const_iterator i = elems.begin();
    i != elems.end();
        ++i)
    {
        (*i)->draw(cr);
    }
    cairo_show_page(cr);
}

void Canvas::show() const
{
    draw();
    win->show();
}

void Canvas::startLoop()
{
    win->loop();
}
} // namespace exercise
