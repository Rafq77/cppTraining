/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "allocmock.hh"

#include "lest.hpp"
#include "trompeloeil.hpp"

#include "test-classes.hh"
#include <iostream>

#include "text.hh"

namespace
{
using namespace exercise;

class GraphErrorReporter
{
public:
    GraphErrorReporter() = default;

    void setError(char const *file, unsigned line, std::string const &msg)
    {
        // we only report the first error
        if (err) return;
        srcFile = file;
        srcLine = line;
        errMsg = msg;
        err = true;
    }

    bool ok()
    {
        if (err)
        {
            std::clog << "Problem at " << srcFile << ':' << srcLine << '\n';
            std::clog << errMsg;
            err = false;
            return false;
        }
        return true;
    }

private:
    char const *srcFile = nullptr;
    unsigned srcLine = 0;
    std::string errMsg;
    bool err = false;
};
GraphErrorReporter errReporter;

} // unnamed namespace

namespace exerciseTest
{
bool testOk()
{
    return errReporter.ok();
}

using exercise::Text;
using trompeloeil::_;

const lest::test generalTests[] =
{
    {CASE("Shape position correctly initialized")
     {
         ShapeTest t1;
         ShapeTest t2("T2", Position(250, -36.5), Color(15.2, -66, 0), Pen(5E77));
         EXPECT(t1.getPosition().x == 0);
         EXPECT(t2.getPosition().y == -36.5);
     }},
    {CASE("Test Text::setName w/lvalue")
     {
         int allocCnt1, allocCnt2, allocCnt3;
         std::string s1{"some long string that hopefully is too long for small string optimization"};
         std::string s2;
         Text testObj{s2, s2};
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management
             {
                 ALLOW_CALL(tracer, trackNew(_, _));
                 ALLOW_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 allocCnt1 = testAlloc.getCount();
                 s2 = s1;
                 allocCnt2 = testAlloc.getCount() - allocCnt1;
                 testObj.setText(s1);
                 allocCnt3 = testAlloc.getCount() - (allocCnt2 + allocCnt1);
                 EXPECT(allocCnt3 == allocCnt2);
             }
             EXPECT(testOk());
             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
     }},
#ifdef HAS_TEXT_FORWARD_SET_NAME
    {CASE("Test Text::setName w/rvalue")
     {
         int allocCnt1, allocCnt2;
         std::string s1{"some long string that hopefully is too long for small string optimization"};
         std::string s2;
         Text testObj{s2, s2};
         AllocTracerMock tracer;
         AllocMock testAlloc;
         { // we need another scope for memory management
             {
                 ALLOW_CALL(tracer, trackNew(_, _));
                 ALLOW_CALL(tracer, trackDelete(_));

                 TraceSetter ts{testAlloc, &tracer};

                 allocCnt1 = testAlloc.getCount();
                 testObj.setText(std::move(s1));
                 allocCnt2 = testAlloc.getCount();
                 EXPECT(allocCnt1 == allocCnt2);
             }
             EXPECT(testOk());
             // now all acquired resources should be released
         }
         EXPECT(testAlloc.ok());
     }},
#endif
};

} // namespace exerciseTest

int main(int argc, char *argv[])
{
    int status = 0;
    trompeloeil::set_reporter(
        [] (trompeloeil::severity s, char const *file, unsigned line, std::string const &msg)
        {
            std::cerr << "Problem at " << file << ':' << line << '\n';
            std::cerr << msg;
            if (s == trompeloeil::severity::fatal)
            {
                throw std::runtime_error("Fatal problem");
            } else {
                errReporter.setError(file, line, msg);
            }
        });

    status += lest::run(exerciseTest::generalTests, argc, argv, std::cerr);

    status += lest::run(lest::tests{exerciseTest::groupTests, exerciseTest::groupTests+exerciseTest::groupTestNum}, argc, argv, std::cerr);

    status += lest::run(lest::tests{exerciseTest::canvasTests, exerciseTest::canvasTests+exerciseTest::canvasTestNum}, argc, argv, std::cerr);

#ifdef _WIN32
    status += lest::run(lest::tests{exerciseTest::w32winTests, exerciseTest::w32winTests+exerciseTest::w32winTestNum}, argc, argv, std::cerr);
#else
    status += lest::run(lest::tests{exerciseTest::xwinTests, exerciseTest::xwinTests+exerciseTest::xwinTestNum}, argc, argv, std::cerr);
#endif

    std::clog << status << " test suites failed.\n";

    return status;
}
