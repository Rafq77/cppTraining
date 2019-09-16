/*
 * Copyright (c) 2014-2019 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "staticVector.hh"
#include "staticVector.tt"

#include <string>
#include <memory>
#include <iostream>

using namespace std::literals;
using namespace exercise;

using std::cout;

void foo1()
{
    StaticVector<std::string> v1{ "Hello"s, "C++"s };

    auto v2 = v1;

    cout << "Equal: " << std::boolalpha << (v1 == v2) << '\n';

    for (std::string const &s: v2)
    {
        cout << s << ' ';
    }
    cout << '\n';

    try {
        cout << "Deliberate out of range access: " << v1[v1.size()+1];
    } catch (std::out_of_range exception)
    {
        cout << "Could not access an out of range index:\n" << exception.what() << "\n";
        throw;
    }
}

int main()
{
    std::string *sPtr =  new std::string("asd");
    delete sPtr;
    cout << *sPtr; // deliberate to have the sanitizer here
     
//    try { 
        foo1();
 //   } catch (std::exception e) {
   //     std::terminate();
  //  }

    return 0;
}
