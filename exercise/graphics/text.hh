/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TEXT_HH_SEEN_
#define TEXT_HH_SEEN_
#include "shape.hh"
#include <string>

namespace exercise
{
class Text : public Shape
{
public:
    Text(std::string const &txt
         , std::string const &name
	 , Position pos = Position()
	 , Color c = Color()
	 , Pen pen = Pen());

    void setText(std::string const &);

private:
    void doDraw(cairo_t *cr) const override;

    std::string t;
};
} // namespace exercise
#endif /* TEXT_HH_SEEN_ */
