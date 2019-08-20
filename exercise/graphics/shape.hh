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
    Color() = default;
    Color(double red, double green, double blue);

    double rgbR{0};
    double rgbG{0};
    double rgbB{0};

    static Color Black;
    static Color White;
};

struct Pen
{
    Pen() = default;
    //Pen(Pen &&p) = default;
    Pen(double width);

    double w{2};
};

struct Position
{
    Position() = default;
    //Position(Position &&p) = default;
    Position(double xPos, double yPos);

    double x{0};
    double y{0};
};


class Shape
{
public:
    Shape() = default;
    Shape(Shape &s) = default;
    Shape(Shape &&s) = default;
    Shape& operator=(Shape &&s) = default;
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
    Position xy = Position{};

private:
    virtual void doDraw(cairo_t *context) const = 0;

    Color clr = Color{};
    Pen p = Pen{};
    std::string name{};
};
} // namespace exercise
#endif /* SHAPE_HH_SEEN_ */
