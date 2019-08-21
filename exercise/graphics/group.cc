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
  : Shape{name}
{
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
    apply([=](UPShape& s) { s->move(relX, relY); });
}

void Group::setColor(Color clr)
{
    Shape::setColor(clr);
	apply([=](UPShape& s ){s->setColor(clr);});
}

void Group::setPen(Pen p)
{
    Shape::setPen(p);
	apply([&](UPShape& s ){s->setPen(p);});
}

void Group::addChild(Shape *s)
{
    children.emplace_back(s);
}

void Group::doDraw(cairo_t *context) const
{
	for (auto& c : children)
	{
		c->draw(context);
	}
}

} // namespace exercise
