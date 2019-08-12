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

STD ?= -std=c++2a
COMMFLAGS = -Wall $(STD) -g $(OPT)

#TC ?= LLVM
TC ?= GCC

CXXFLAGS += $(COMMFLAGS) $(INCLUDES)

HEADERS ?= $(wildcard *.hh)
SOURCES ?= $(wildcard *.cc)
OBJS ?= $(patsubst %.cc,%.o,$(SOURCES))

ENVCXX := $(shell echo "$$CXX")

ifeq ($(TC), GCC)
ifeq ($(ENVCXX),)
CXX = g++
endif
endif

ifeq ($(TC), LLVM)
ifeq ($(ENVCXX),)
CXX = clang++
endif
STD += -stdlib=libc++
LIBS += -lc++abi
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


$(OBJS):: $(HEADERS) $(EXTRA_OBJ_DEPS)

clean: $(CLEAN_EXTRA_DEP)
	rm -f $(TARGET) $(OBJS) $(PROGS) $(TARGETS) $(CLEAN_EXTRA)
	rm -f *.o m.depend core out.txt *.ti *.ii *.plg *.ncb *.opt *~
