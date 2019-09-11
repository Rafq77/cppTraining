/*
 * Copyright (c) 2014-2019 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "canvas.hh"

#include "pathShape.hh"
#include "rectangle.hh"
#include "text.hh"

#include "extgraph.hh"
#include <string>
#include <vector>
#include <functional>

namespace exercise
{
CanvasImpl::CanvasImpl(int width, int height, std::string const &name)
  : win{new GuiWin(width, height, name)}
  , surface{win->getSurface(), cairo_surface_destroy}
  , cr{cairo_create(surface.get()), [](cairo_t *c) { cairo_destroy(c);} }
  //, helperCallback{[t=this](){ t->draw();}}   // attempt, not final solution
  , helperClass{[w=win.get()](){ w->unregisterCallback();}}  // for cleaniness i wanted to use the helper class for callback management. But instead, use the helper to unregister only.
  //: win{new GuiWin(width, height, name), [](GuiWin* g){ g->unregisterCallback(); delete g; }} // this is not effective, because unregister is being called too late!
  //, surface{win->getSurface(), [](cairo_surface_t *c) {cairo_surface_destroy(c);} }
  //, cr{cairo_create(surface.get()), cairo_destroy} // works as well
  //, cr{cairo_create(surface.get()), [&](cairo_t *c) { this->win->uniregisterCallback(); cairo_destroy(c);} } // Problem, because I am using ´this´, which will be invalid after moving the object.
{
    win->registerCallback([t=this]() { t->draw(); });
    
    (*this) += new PathShape(new Rectangle(width, height), "Background", true, Position(0, 0), Color::White);
    show();
}

CanvasImpl::CanvasImpl(CanvasImpl&& other) 
	: win{std::move(other.win)}
	, surface{std::move(other.surface)}
	, cr{std::move(other.cr)}
	, elems{std::move(other.elems)}
	, shapeMap{std::move(other.shapeMap)}
{

	if (win)
	{
		win->unregisterCallback(); 
		win->registerCallback([t=this]() { t->draw(); });
	}

	other.elems.clear();
}

CanvasImpl& CanvasImpl::operator=(CanvasImpl&& other) 
{
	CanvasImpl tmp{std::move(other)};

	std::swap(win, tmp.win);
	std::swap(surface, tmp.surface);
	std::swap(cr, tmp.cr);
	std::swap(elems, tmp.elems);
	std::swap(shapeMap, tmp.shapeMap);

	if (win) 
	{
			win->unregisterCallback(); 
			win->registerCallback([t=this]() { t->draw(); });
	}

	return *this;
};

void CanvasImpl::operator+=(Shape *s)
{
    elems.emplace_back(s);
	shapeMap[elems.back()->getName()] = elems.back().get();
}

void CanvasImpl::draw() const
{
	for (auto &s : elems)
	{
		s->draw(cr.get());
	}

    cairo_show_page(cr.get());
}

void CanvasImpl::show() const
{
    draw();
    win->show();
}

void CanvasImpl::startLoop()
{
    win->loop();
}

OptionalShapeRef CanvasImpl::getShape(const std::string& name)
{
	if (auto shapeIt = shapeMap.find(name); shapeIt != shapeMap.end()) 
	{
		auto const & [key, value] = *shapeIt;
		return OptionalShapeRef(*value); 
	} else 
	{
		return OptionalShapeRef();
	}
}

/////////////////// Canvas class

Canvas::Canvas(int width, int height, std::string const &name)
  : pImpl{new CanvasImpl{width, height, name}}
{
}

void Canvas::operator+=(Shape *s)
{
    (*pImpl) += s;
}

void Canvas::draw() const
{
    pImpl->draw();
}

void Canvas::show() const
{
    pImpl->show();
}

void Canvas::startLoop()
{
    pImpl->startLoop();
}

OptionalShapeRef Canvas::getShape(const std::string& name)
{
	return pImpl->getShape(name);
}


} // namespace exercise
