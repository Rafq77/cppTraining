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
  : win(new GuiWin(width, height, name))
  , surface(win->getSurface())
  , cr(cairo_create(surface))
{
    //win->registerCallback([t=this]() { draw(); } );
    win->registerCallback([t=this]() { t->draw(); });
    
    (*this) += new PathShape(new Rectangle(width, height), "Background", true, Position(0, 0), Color::White);
    show();
}

CanvasImpl::CanvasImpl(CanvasImpl&& other) 
	: win{std::move(other.win)}
	, surface{std::move(other.surface)}
	, cr{std::move(other.cr)}
	, elems{std::move(other.elems)}
{

	if (win)
	{
		win->unregisterCallback(); 
		win->registerCallback([t=this]() { t->draw(); });
	}

	other.win = nullptr;
	other.surface = nullptr;
	other.cr = nullptr;
	other.elems.clear();

}

CanvasImpl& CanvasImpl::operator=(CanvasImpl&& other) 
{
	CanvasImpl tmp{std::move(other)};

	std::swap(win, tmp.win);
	std::swap(surface, tmp.surface);
	std::swap(cr, tmp.cr);
	std::swap(elems, tmp.elems);

	if (win) 
	{
			win->unregisterCallback(); 
			win->registerCallback([t=this]() { t->draw(); });
	}

	return *this;
};

CanvasImpl::~CanvasImpl()
{
	if (win)
	{
		win->unregisterCallback(); // it's the contract
		cairo_destroy(cr);
		cairo_surface_destroy(surface);
		delete win;
	}
}

void CanvasImpl::operator+=(Shape *s)
{
    elems.emplace_back(s);
}

void CanvasImpl::draw() const
{
	for (auto &s : elems)
	{
		s->draw(cr);
	}

    cairo_show_page(cr);
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



} // namespace exercise
