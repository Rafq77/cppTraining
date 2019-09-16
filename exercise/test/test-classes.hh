/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TEST_CLASSES_HH_SEEN_
#define TEST_CLASSES_HH_SEEN_

#include "trompeloeil.hpp"
#include "lest.hpp"

#include <shape.hh>

namespace exerciseTest
{
using exercise::Shape;
using exercise::Position;

class ShapeTest : public Shape
{
public:
    using Shape::Shape;

    Position getPosition() const
    {
        return xy;
    }

private:
    void doDraw(cairo_t *) const override
    {
    }

};

class ShapeMock : public Shape
{
public:
    using Shape::Shape;

    MAKE_CONST_MOCK1(doDraw, void(cairo_t *));

};

bool testOk();

extern const lest::test canvasTests[];
extern const size_t canvasTestNum;

extern const lest::test w32winTests[];
extern const size_t w32winTestNum;

extern const lest::test xwinTests[];
extern const size_t xwinTestNum;

extern const lest::test groupTests[];
extern const size_t groupTestNum;
} // namespace exerciseTest
#endif /*  TEST_CLASSES_HH_SEEN_ */
