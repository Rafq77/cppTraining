/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CIRCLE_HH_SEEN_
#define CIRCLE_HH_SEEN_
#include "pathBase.hh"

namespace exercise
{
class Circle : public PathBase
{
public:
    explicit Circle(double radius);

    void preparePath(cairo_t *cr, Position xy) const;

private:
    double r;
};
} // namespace exercise
#endif /* CIRCLE_HH_SEEN_ */
