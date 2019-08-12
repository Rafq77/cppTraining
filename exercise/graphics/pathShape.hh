/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef PATH_SHAPE_HH_SEEN_
#define PATH_SHAPE_HH_SEEN_
#include "shape.hh"
#include "pathBase.hh"

namespace exercise
{
class PathShape : public Shape
{
public:
    PathShape(PathBase *path
              , std::string const &name
              , bool fill
              , Position pos = Position()
              , Color c = Color()
              , Pen pen = Pen());
    ~PathShape();

private:
    // no copy, as we can't copy 'path'
    PathShape(PathShape const&);
    PathShape &operator=(PathShape const&);

    virtual void doDraw(cairo_t *cr) const;

    PathBase *path;
    bool fill;
};
} // namespace exercise
#endif /* PATH_SHAPE_HH_SEEN_ */
