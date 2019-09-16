/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "shape.hh"

#include "extgraph.hh"

namespace exercise
{

Color::Color(double red, double green, double blue)
  : rgbR{red}
  , rgbG{green}
  , rgbB{blue}
{
}

Color Color::Black{0, 0, 0};
Color Color::White{1, 1, 1};

Pen::Pen(double width)
  : w{width}
{
}

Position::Position(double xPos, double yPos)
  : x{xPos}
  , y{yPos}
{
}

Shape::Shape(std::string const &name_)
  : name{name_}
{
}

Shape::Shape(std::string const &name_, Position pos, Color c, Pen pen)
  : xy{pos}
  , clr{c}
  , p{pen}
  , name{name_}
{
}

void Shape::draw(cairo_t *cr) const
{
    cairo_save(cr);
    cairo_move_to(cr, xy.x, xy.y);
    cairo_set_source_rgb(cr, clr.rgbR, clr.rgbG, clr.rgbB);
    cairo_set_line_width(cr, p.w);
    doDraw(cr);
    cairo_restore(cr);
}

std::string const &Shape::getName() const
{
    return name;
}

void Shape::setName(std::string const &name_)
{
    name = name_;
}

void Shape::setPosition(Position pos)
{
    xy = pos;
}

void Shape::move(double relX, double relY)
{
    xy.x += relX;
    xy.y += relY;
}

void Shape::setColor(Color c)
{
    clr = c;
}

void Shape::setPen(Pen pen)
{
    p = pen;
}
} // namespace exercise
