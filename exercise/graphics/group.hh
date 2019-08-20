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

#include <functional>
#include "group-test.hh"

namespace exercise
{


class Group : public Shape
{
public:
    Group() = default;
    Group(std::string const &name);
    Group(Group const &) = delete;
    Group &operator=(Group const &) = delete;
    ~Group();

    void setPosition(Position) override;
    void move(double relX, double relY) override;
    void setColor(Color) override;
    void setPen(Pen) override;

    void addChild(Shape *);

private:

	/*
	std::function<void(Color clr)> apply = [&](Color clr) { 
		for (Shape* s : children)
		{
			s->setColor(clr); 
		}
	};
	*/

	template <class F>
	void apply(F f)
	{
		for (Shape *s : children)
		{
			f(s);
		}
	}
	
    virtual void doDraw(cairo_t *context) const override;

    std::vector<Shape *> children;
    //typedef decltype(children.begin()) SIter; // this would be a reference
    typedef std::vector<Shape *>::iterator SIter;

public:
    typedef decltype(children)::value_type testElemType;
};
} // namespace exercise
#endif /* GROUP_HH_SEEN_ */
