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
    // Ro0 - no std ctors and dtor
	Group() = default;
	Group(Group&) = default; // cctor
	Group(Group&&) = default; //mvctor
	Group& operator=(Group&) = default; // cassign
	Group& operator=(Group&&) = default; // massign
	~Group() = default;

    Group(std::string const &name);
    
    void setPosition(Position) override;
    void move(double relX, double relY) override;
    void setColor(Color) override;
    void setPen(Pen) override;

    void addChild(Shape *);

private:

	template <class F>
	void apply(F f)
	{
		for (auto& s : children)
		{
			f(s);
		}
	}
	
    virtual void doDraw(cairo_t *context) const override;

	// UPShape def moved to shape.hh
    std::vector<UPShape> children;

    //typedef decltype(children.begin()) SIter; // this would be a reference
    typedef std::vector<UPShape>::iterator SIter;

public:
    typedef decltype(children)::value_type testElemType;
};

} // namespace exercise
#endif /* GROUP_HH_SEEN_ */
