/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SHAPE_HH_SEEN_
#define SHAPE_HH_SEEN_

#include "extgraph.hh" // we need cairo_t

#include <string>

namespace exercise
{
struct Color
{
    Color();
    Color(double red, double green, double blue);

    double rgbR;
    double rgbG;
    double rgbB;

    static Color Black;
    static Color White;
};

struct Pen
{
    Pen();
    Pen(double width);

    double w;
};

struct Position
{
    Position();
    Position(double xPos, double yPos);

    double x;
    double y;
};


class Shape
{
public:
    Shape();
    Shape(std::string const &name);
    Shape(std::string const &name, Position, Color, Pen);
    virtual ~Shape() {}

    void draw(cairo_t *cr) const;

    std::string const &getName() const;
    void setName(std::string const &name);

    virtual void setPosition(Position);
    virtual void move(double relX, double relY);
    virtual void setColor(Color);
    virtual void setPen(Pen);

protected:
    Position xy;

private:
    virtual void doDraw(cairo_t *context) const = 0;

    Color clr;
    Pen p;
    std::string name;
};
} // namespace exercise
#endif /* SHAPE_HH_SEEN_ */
