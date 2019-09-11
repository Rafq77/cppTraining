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

#include <optional>
#include <vector>
#include <string>


namespace exercise
{

struct CanvasHelperClass
{
		~CanvasHelperClass()
		{
				fn();
		}
		std::function<void()> fn = [](){};
};

class CanvasImpl
{
public:
	CanvasImpl(CanvasImpl&& other);
    CanvasImpl(CanvasImpl const&) = delete;
	CanvasImpl& operator=(CanvasImpl&& other);
    CanvasImpl& operator=(CanvasImpl const&) = delete;
    CanvasImpl(int width, int height, std::string const &name);

    void operator+=(Shape *);

    void draw() const;
    void show() const;
    void startLoop();

	OptionalShapeRef getShape(const std::string& name);

private:

    std::unique_ptr<GuiWin> win; 
    std::unique_ptr<cairo_surface_t, std::function<void(cairo_surface_t *)>> surface;
    std::unique_ptr<cairo_t, std::function<void(cairo_t *)>> cr;
    std::vector<UPShape> elems;
	std::unordered_map<std::string, Shape*> shapeMap; // name is key, val is the address from vector of unique_ptr 
	// CanvasHelperClass helperCallback;
	CanvasHelperClass helperClass;
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

	OptionalShapeRef getShape(const std::string& name);

    void operator+=(Shape *);

    void draw() const;
    void show() const;
    void startLoop();

private:
	std::unique_ptr<CanvasImpl> pImpl;
};

} // namespace exercise
#endif /* CANVAS_HH_SEEN_ */
