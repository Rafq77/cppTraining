/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

// This wraps group.cc for testing to use our vector mocks
// the magic is in the mocking header file
#define SPECIAL_VECTOR_MOCK

#include "group.cc"
