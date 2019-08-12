/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "group.hh"

#include "shape.hh"

namespace exercise
{
Group::Group()
{
}

Group::Group(std::string const &name)
  : Shape(name)
{
}

Group::~Group()
{
    for (SIter i = children.begin(); i != children.end(); ++i)
    {
        delete (*i);
    }
}

void Group::setPosition(Position newPos)
{
    double relX = newPos.x - xy.x;
    double relY = newPos.y - xy.y;

    move(relX, relY);
}

void Group::move(double relX, double relY)
{
    Shape::move(relX, relY);
    for (SIter i = children.begin(); i != children.end(); ++i)
    {
        (*i)->move(relX, relY);
    }
}

void Group::setColor(Color clr)
{
    Shape::setColor(clr);
    for (SIter i = children.begin(); i != children.end(); ++i)
    {
        (*i)->setColor(clr);
    }
}

void Group::setPen(Pen p)
{
    Shape::setPen(p);
    for (SIter i = children.begin(); i != children.end(); ++i)
    {
        (*i)->setPen(p);
    }
}

void Group::addChild(Shape *s)
{
    children.push_back(s);
}

void Group::doDraw(cairo_t *context) const
{
    for (std::vector<Shape *>::const_iterator i = children.begin();
         i != children.end();
         ++i)
    {
        (*i)->draw(context);
    }
}
} // namespace exercise
