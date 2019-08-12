/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "text.hh"

#include "extgraph.hh"

namespace exercise
{
Text::Text(std::string const &txt
           , std::string const &name
           , Position pos
           , Color c
           , Pen pen)
  : Shape(name, pos, c, pen)
  , t(txt)
{
}

void Text::doDraw(cairo_t *cr) const
{
    cairo_show_text(cr, t.c_str());
}

void Text::setText(std::string const &txt)
{
    t = txt;
}
} // namespace exercise
