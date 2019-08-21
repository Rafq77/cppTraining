/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CANVAS_HH_SEEN_
#define CANVAS_HH_SEEN_
#include "shape.hh"
#include "extgraph.hh"
#include "xwin.hh"

#include <vector>
#include <string>


namespace exercise

{
class CanvasImpl
{
public:
	CanvasImpl(CanvasImpl&& other);
    CanvasImpl(CanvasImpl const&) = delete;
	CanvasImpl& operator=(CanvasImpl&& other);
    CanvasImpl& operator=(CanvasImpl const&) = delete;
    CanvasImpl(int width, int height, std::string const &name);
    ~CanvasImpl();

    void operator+=(Shape *);

    void draw() const;
    void show() const;
    void startLoop();

private:

    GuiWin *win;
    cairo_surface_t *surface;
    cairo_t *cr;
    std::vector<UPShape> elems;
};

class Canvas
{
public:
    Canvas(int width, int height, std::string const &name);
    Canvas() = default;
    Canvas(Canvas&) = default;
    Canvas(Canvas&&) = default;
    Canvas& operator=(Canvas&) = default;
    Canvas& operator=(Canvas&&) = default;
    ~Canvas() = default;

    Canvas(Canvas const&) = delete;
    Canvas& operator=(Canvas const&) = delete;

    void operator+=(Shape *);

    void draw() const;
    void show() const;
    void startLoop();

private:
	std::unique_ptr<CanvasImpl> pImpl;
};

} // namespace exercise
#endif /* CANVAS_HH_SEEN_ */
