/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

// This wraps canvas.cc for testing to use our (w32|x)win mocks
// the magic is in the mocking header files
#ifdef _WIN32
#include "w32win-mock.hh"
#else
#include "xwin-mock.hh"
#endif

#include "canvas.cc"
