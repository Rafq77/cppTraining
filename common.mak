# common make rules for workshop

# for now we support a single TARGET with OBJS,
# and multiple SRCS that each create a separate binary
# multpile PROGS are cleaned, but they must be built in their respective
# Makefiles

# Copyright (c) 2003-2019 Detlef Vollmann.  Permission to copy, use, modify,
# sell and distribute this software is granted provided this copyright
# notice appears in all copies.

# This software is provided "as is" without express or implied
# warranty, and with no claim as to its suitability for any purpose.

ifeq ($(TESTDIR),)
-include env.mak
else
-include $(TESTDIR)/env.mak
endif

M4_INCLUDE ?= -I$(TOPDIR)/../common

STD ?= -std=c++2a
#STD ?= -std=c++17
COMMFLAGS = -Wall $(STD) -g $(OPT)

#TC ?= LLVM
TC ?= GCC
GCC=gcc-8

CXXFLAGS += $(COMMFLAGS) $(INCLUDES)
CXXFLAGS += -fsanitize=address
#CXXFLAGS += -fsanitize=undefined

HEADERS ?= $(wildcard *.hh)
SOURCES += $(wildcard *.cc)
OBJS ?= $(patsubst %.cc,%.o,$(sort $(SOURCES)))

ENVCXX := $(shell echo "$$CXX")

ifeq ($(TC), GCC)
ifeq ($(ENVCXX),)
CXX = g++-8
CC = gcc
endif
endif

ifeq ($(TC), LLVM)
ifeq ($(ENVCXX),)
CXX = clang++
CC = clang
endif
STD += -stdlib=libc++
LIBS += -lc++abi 
CXXFLAGS += -D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES
endif

LD = $(CXX)
LIBS += 
LDLIBS = $(LIBDIRS) $(LIBS)
# default rule uses $(CC) and $(LDLIBS)
LINK.o = $(LD) $(COMMFLAGS)
# LINK.cc is fine

.SHELLFLAGS = -ec

TARGETS += $(patsubst %.cc, %, $(SRCS))

all: debug

debug: OPT = -O0
release: OPT = -O3

debug release: $(TARGET) $(TARGETS)

$(TARGET): $(OBJS)
	$(LINK.cc) $(OBJS) $(LDLIBS) -o $@

%.cc: %.m4
	m4 $(M4_VARS) $(M4_INCLUDE) $< > $@


$(OBJS):: $(HEADERS) $(EXTRA_OBJ_DEPS)

clean: $(CLEAN_EXTRA_DEP)
	rm -f $(TARGET) $(OBJS) $(PROGS) $(TARGETS) $(CLEAN_EXTRA)
	rm -f *.o m.depend core out.txt *.ti *.ii *.plg *.ncb *.opt *~

distclean: clean
	rm -f $(CLEAN_DIST)
