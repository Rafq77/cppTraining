/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef GROUP_HH_SEEN_
#define GROUP_HH_SEEN_
#include "shape.hh"
#include <vector>

#include "group-test.hh"

namespace exercise
{
class Group : public Shape
{
public:
    Group();
    Group(std::string const &name);
    ~Group();

    virtual void setPosition(Position);
    virtual void move(double relX, double relY);
    virtual void setColor(Color);
    virtual void setPen(Pen);

    void addChild(Shape *);

private:
    Group(Group const &);
    Group &operator=(Group const &);

    virtual void doDraw(cairo_t *context) const;

    std::vector<Shape *> children;
    //typedef decltype(children.begin()) SIter; // MSVC doesn't like this
    typedef std::vector<Shape *>::iterator SIter;

public:
    typedef decltype(children)::value_type testElemType;
};
} // namespace exercise
#endif /* GROUP_HH_SEEN_ */
