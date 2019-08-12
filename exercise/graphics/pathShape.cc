/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "pathShape.hh"

namespace exercise
{
PathShape::PathShape(PathBase *shape
                     , std::string const &name
                     , bool doFill
                     , Position pos
                     , Color c
                     , Pen pen)
  : Shape(name, pos, c, pen)
  , path(shape)
  , fill(doFill)
{
}

PathShape::~PathShape()
{
    delete path;
}

void PathShape::doDraw(cairo_t *cr) const
{
    path->preparePath(cr, xy);
    if (fill)
    {
        cairo_fill(cr);
    }
    else
    {
        cairo_stroke(cr);
    }
}
} // namespace exercise
