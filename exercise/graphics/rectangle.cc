/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "rectangle.hh"

#include "extgraph.hh"

namespace exercise
{
Rectangle::Rectangle(double width, double height)
  : w(width)
  , h(height)
{
}

void Rectangle::preparePath(cairo_t *cr, Position xy) const
{
    cairo_rectangle(cr, xy.x, xy.y, w, h);
}
} // namespace exercise
