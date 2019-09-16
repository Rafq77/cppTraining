/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
#include <iterator>

using std::cout;

typedef std::pair<int, int> IntPair;
std::vector<IntPair> testArray;

void countTheTwos()
{
    cout << std::count_if(testArray.begin(),
                          testArray.end(),
                          [] (IntPair const &p) {
                              return p.first == 2; })
         << '\n';
}

class X
{
public:
    int countMePlus()
    {
        return count_if(testArray.begin(),
                        testArray.end(),
                        [this] (IntPair const &p) {
                            return p.first == myVal;
                        });
    }
private:
        int myVal;
};

template <typename Container>
void raise(Container &c, double percentage)
{
    typedef typename Container::value_type ValT;

    std::for_each(c.begin(), c.end(),
                  [percentage] (ValT &v)
                  {
                      auto add = v * percentage / 100;
                      v += add;
                  });
}

float runFoo(std::function<float()> f)
{
    return f();
}

void handleData()
{
    int faults = 0;
    int samples = 0;
    auto faultRate = [&] {
        return float(faults) / samples;
    };
// ...
    cout << "Current fault rate: "
         << faultRate() << '\n';

    runFoo(faultRate);
    runFoo([=] { return faultRate(); });
}

void plot(std::function<float(float)> foo)
{
// ...
}

template <class F>
void plot(F foo)
{
    // ...
}

struct X2
{
    void f()
    {
        plot([r=factor] (float y) { return r * y; });
    }

    float factor = 2.7;
};

// generic lambda
auto times1 = [] (auto x, auto y) { return x * y; };

// esentially equivalent to:
template <class X, class Y>
auto times2(X x, Y y)
{
    return x * y;
}

int main()
{
    std::vector<double> v{27653, 38913, 19933, 42478};
    std::copy(v.begin(), v.end(), std::ostream_iterator<double>(cout, " "));
    cout << '\n';

    raise(v, 1.5);
    std::copy(v.begin(), v.end(), std::ostream_iterator<double>(cout, " "));
    cout << '\n';

    return 0;
}
