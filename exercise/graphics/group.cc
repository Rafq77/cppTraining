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

Group::Group(std::string const &name)
  : Shape(name)
{
}

Group::~Group()
{
	//for (Shape* s : children)
	for (auto s : children) // pointer
	{
		delete s;
	}
}

void Group::setPosition(Position newPos)
{
    auto relX = newPos.x - xy.x;
    auto relY = newPos.y - xy.y;

    move(relX, relY);
}

void Group::move(double relX, double relY)
{
    Shape::move(relX, relY);
    apply([=](Shape *s) { s->move(relX, relY); });
	// TODO
	/*
	for (auto &s : children) // pointer on shape
	{
		s->move(relX, relY);
	}
	*/
}

void Group::setColor(Color clr)
{
    Shape::setColor(clr);
	apply([=](Shape *s ){s->setColor(clr);});
}

void Group::setPen(Pen p)
{
    Shape::setPen(p);
	apply([&](Shape *s ){s->setPen(p);});
}

void Group::addChild(Shape *s)
{
    children.push_back(s);
}

void Group::doDraw(cairo_t *context) const
{
    for (auto i = children.begin();
         i != children.end();
         ++i)
    {
        (*i)->draw(context);
    }
}
} // namespace exercise
