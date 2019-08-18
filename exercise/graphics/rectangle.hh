/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RECTANGLE_HH_SEEN_
#define RECTANGLE_HH_SEEN_
#include "pathBase.hh"

namespace exercise
{
class Rectangle : public PathBase
{
public:
    Rectangle(double width, double height);

    void preparePath(cairo_t *cr, Position xy) const override;

private:
    double w;
    double h;;
};
} // namespace exercise
#endif /* RECTANGLE_HH_SEEN_ */
