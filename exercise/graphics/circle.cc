/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "circle.hh"

#include "extgraph.hh"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265
#endif

namespace exercise
{
Circle::Circle(double radius)
  : r(radius)
{
}

void Circle::preparePath(cairo_t *cr, Position xy) const
{
    cairo_new_sub_path(cr);
    cairo_arc(cr, xy.x, xy.y, r, 0, 2*M_PI);
}
} // namespace exercise
