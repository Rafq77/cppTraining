/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef PATH_BASE_HH_SEEN_
#define PATH_BASE_HH_SEEN_
#include "shape.hh"

namespace exercise
{
class PathBase
{
public:
    virtual ~PathBase() {}
    virtual void preparePath(cairo_t *cr, Position xy) const = 0;
};
} // namespace exercise
#endif /* PATH_BASE_HH_SEEN_ */
