/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "canvas.hh"
#include "circle.hh"
#include "text.hh"
#include "group.hh"
#include "rectangle.hh"
#include "pathShape.hh"

#include <unistd.h>
#include <vector>
#include <utility>
#include <exception>
#include <thread>
#include <chrono>

#define GROUP_MOVE 1
#define CANVAS_MOVE 1

using namespace exercise;
// using namespace std::literals;
// using std::literals::s;
constexpr std::chrono::seconds sec1(1);

Group& getShapeFromOptional(OptionalShapeRef optRef)
{
	if (optRef) 
	{
		return dynamic_cast<Group&>(optRef->get()); // reference wrapper 
	}
	throw std::runtime_error("Extracting from shape went horribly wrong!");
}

// template could go here
Group& getShapeFromOptional(Canvas& c, std::string name)
{
    OptionalShapeRef optRef = c.getShape(name);
	if (optRef) 
	{
		return dynamic_cast<Group&>(optRef->get()); // reference wrapper 
	}
	throw std::runtime_error("Extracting from shape went horribly wrong!");
}

#ifdef CANVAS_MOVE
std::vector<Canvas> allC;

void showAndAdd(Canvas &&c)
{
    using std::move;

    c.show();
    allC.push_back(move(c));
    std::this_thread::sleep_for(sec1);
}

void showAll()
{
    for (Canvas const &cv: allC)
    {
        cv.show();
    }
}

void circleWin()
{
    using std::move;

    Canvas c(320, 240, "Circle Win");

    Shape *crcl = new PathShape{new Circle{100}, "Circle", true
                              , Position{160, 120}
                              , Color{0, 0, 0.5}};
    c += crcl;

    Text *txt = new Text{"Blue Circle", "Text 1", Position(10, 20)};
    Shape *frame = new PathShape{new Rectangle(70, 30), "Frame 1", false};

    Group *g = new Group{"Frame Group 1"};
    g->addChild(txt);
    g->addChild(frame);

    g->move(10, 200);
    c += g;

    showAndAdd(move(c));
}

void textWin()
{
    using std::move;

    Canvas c{200, 100, "Text Win 1"};
    Text *txt1 = new Text{"Hello", "Text 2", Position(30, 20)};
    Shape *frame = new PathShape{new Rectangle(90, 30), "Frame 2", false};
    Text *txt2 = new Text(*txt1);
    txt2->setName("Text 3");
    txt2->setText("World!");
    txt2->setPosition(Position(150, 70));

    Group *g = new Group{"Frame Group 2"};
    g->addChild(txt1);
    g->addChild(frame);
    g->move(10, 10);
    c += g;

    c += txt2;

    showAndAdd(move(c));
}
#endif

int main()
{
    Canvas c1{640, 480, "Cairo Example"};

    Shape *crcl = new PathShape{new Circle(100), "Circle", true, Position(320, 240), Color(0, 0, 0.5)};
    c1 += crcl;
    c1.show();
    std::this_thread::sleep_for(sec1);

    crcl->setColor(Color{0.5, 0, 0});
    c1.show();
    std::this_thread::sleep_for(sec1);

    Text txtBase{"Hello", "TextBase", Position(20, 20)};
    Rectangle frameBase{70, 30};

    Text *txt = new Text{txtBase};
    txt->setText("Hello 1");
    txt->setName("Text 1");
    Shape *frame = new PathShape{new Rectangle(frameBase), "Frame 1", false};
    Group *g = new Group{"Group 1"};
    g->addChild(txt);
    g->addChild(frame);
    g->move(50, 20);
    c1 += g;

    txt = new Text{txtBase};
    txt->setText("Hello 2");
    txt->setName("Text 2");
    frame = new PathShape{new Rectangle(frameBase), "Frame 2", false};
    g = new Group{"Group 2"};
    g->addChild(txt);
    g->addChild(frame);
    g->move(520, 430);
    c1 += g;

    txt = new Text{txtBase};
    txt->setText("Hello 3");
    txt->setName("Text 3");
    frame = new PathShape{new Rectangle(frameBase), "Frame 3", false};
    g = new Group{"Group 3"};
    g->addChild(txt);
    g->addChild(frame);
    g->move(290, 430);
    c1 += g;

    c1.show();
    sleep(1);

	Group& g1 = getShapeFromOptional(c1.getShape("Group 1"));
	Group& g2 = getShapeFromOptional(c1.getShape("Group 2"));
	Group& g3 = getShapeFromOptional(c1.getShape("Group 3"));
	g1.setColor(Color{1, 0, 0});
    g2.setColor(Color{0, 1, 0});
    g3.setColor(Color{0, 0, 1});

    c1.show();
    sleep(1);

    g1.move(50, 20);
    g2.move(-50, -20);
    g3.move(-60, -300);
    c1.show();

#ifdef GROUP_MOVE
    using std::move;

    c1.show();
    sleep(1);

    Canvas c2(240, 80, "Text Win");
    txt = new Text("Second Text", "Text 5", Position(10, 20));
    frame = new PathShape(new Rectangle(90, 30), "Frame 4", false);
    frame->setColor(Color(0, 1, 0));
    Group *g4 = new Group("Frame Group 4");
    g4->addChild(txt);
    g4->addChild(frame);
    g4->move(20, 15);
    c2 += g4;

    c2.show();
    sleep(1);

    std::swap(g1, *g4);
    c1.show();
    c2.show();
#ifdef CANVAS_MOVE
    showAndAdd(move(c1));

    circleWin();

    textWin();

    allC[2] = move(c2);
    showAll();
    std::this_thread::sleep_for(2*sec1);

    Group &crclTxt = getShapeFromOptional(allC[1], "Group 1");
    crclTxt = move(g3);
    showAll();
    std::this_thread::sleep_for(2*sec1);

    allC[0].startLoop();

    allC.clear();
#endif
#endif

#ifndef CANVAS_MOVE
    c1.startLoop();
#endif

    return 0;
}
