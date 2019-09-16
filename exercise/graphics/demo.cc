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

#define GROUP_MOVE 1
#define CANVAS_MOVE 1

using namespace exercise;

#ifdef CANVAS_MOVE
std::vector<Canvas> allC;

void showAndAdd(Canvas &&c)
{
    using std::move;

    c.show();
    allC.push_back(move(c));
    sleep(1);
}

void showAll()
{
    for (Canvas const &cv: allC)
    {
        cv.show();
    }
}

Group *circleWin()
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

    return g;
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
    sleep(1);

    crcl->setColor(Color{0.5, 0, 0});
    c1.show();
    sleep(1);

    Text txtBase{"Hello", "TextBase", Position(20, 20)};
    Rectangle frameBase{70, 30};

    Text *txt = new Text{txtBase};
    txt->setText("Hello 1");
    txt->setName("Text 1");
    Shape *frame = new PathShape{new Rectangle(frameBase), "Frame 1", false};
    Group *g1 = new Group{"Group 1"};
    g1->addChild(txt);
    g1->addChild(frame);
    g1->move(50, 20);
    c1 += g1;

    txt = new Text{txtBase};
    txt->setText("Hello 2");
    txt->setName("Text 2");
    frame = new PathShape{new Rectangle(frameBase), "Frame 2", false};
    Group *g2 = new Group{"Group 2"};
    g2->addChild(txt);
    g2->addChild(frame);
    g2->move(520, 430);
    c1 += g2;

    txt = new Text{txtBase};
    txt->setText("Hello 3");
    txt->setName("Text 3");
    frame = new PathShape{new Rectangle(frameBase), "Frame 3", false};
    Group *g3 = new Group{"Group 3"};
    g3->addChild(txt);
    g3->addChild(frame);
    g3->move(290, 430);
    c1 += g3;

    c1.show();
    sleep(1);

    g1->setColor(Color{1, 0, 0});
    g2->setColor(Color{0, 1, 0});
    g3->setColor(Color{0, 0, 1});

    c1.show();
    sleep(1);

    g1->move(50, 20);
    g2->move(-50, -20);
    g3->move(-60, -300);
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

    std::swap(*g1, *g4);
    c1.show();
    c2.show();
#ifdef CANVAS_MOVE
    showAndAdd(move(c1));

    Group *crclTxt = circleWin();
    textWin();

    allC[2] = move(c2);
    showAll();
    sleep(2);

    *crclTxt = move(*g3);
    showAll();
    sleep(2);

    allC[0].startLoop();

    allC.clear();
#endif
#endif

#ifndef CANVAS_MOVE
    c1.startLoop();
#endif

    return 0;
}
