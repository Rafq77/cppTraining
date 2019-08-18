# Makefile for C++ workshop tests

MYDIR := $(dir $(lastword $(MAKEFILE_LIST)))

SEARCHDIRS = . .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../..
TOPNAME = $(word 1,$(wildcard $(addsuffix /top.mak,$(SEARCHDIRS))))
include $(TOPNAME)


TESTDEFS =
#TESTDEFS += -DHAS_GROUP_MOVE_CTOR
#TESTDEFS += -DHAS_GROUP_MOVE_ASSIGNMENT
#TESTDEFS += -DHAS_CANVAS_MOVE_CTOR
#TESTDEFS += -DHAS_CANVAS_MOVE_ASSIGNMENT
#TESTDEFS += -DHAS_WIN_MOVE_CTOR
#TESTDEFS += -DHAS_WIN_MOVE_ASSIGNMENT
#TESTDEFS += -DHAS_CANVAS_GET_SHAPE
#TESTDEFS += -DHAS_GROUP_GET_SHAPE
#TESTDEFS += -DHAS_TEXT_FORWARD_SET_NAME
#TESTDEFS += -DHAVE_PATH_SHAPE_TEMPL
#TESTDEFS += -DHAVE_PATH_SHAPE_TEMPL_BOOL

TESTDIR ?= ../graphics
vpath %.hh $(TESTDIR)
vpath %.cc $(TESTDIR)
vpath %.m4 $(TESTDIR)

INCLUDES = -I$(MYDIR) -I$(TESTDIR) -I$(TOPDIR)/include
CXXFLAGS = -DGRAPHMOCK
CXXFLAGS += $(TESTDEFS)

TARGET = test-all

PSSRC = $(wildcard $(TESTDIR)/pathShape*.cc)
PSOBJ = $(notdir $(subst .cc,.o,$(PSSRC)))
PSHDR = $(wildcard $(TESTDIR)/pathShape*.hh)

OBJS = \
   mocks.o \
   test-all.o \
   test-canvas.o \
   test-group.o \
   test-x.o \
   wrap-canvas.o \
   circle.o \
   group.o \
   rectangle.o \
   shape.o \
   text.o \
   xwin.o

OBJS += $(PSOBJ)

TEST_GROUP_SPECIAL_OBJS = \
   test-group-special.o \
   wrap-group.o \
   shape.o

HEADERS = \
   cairomock.hh \
   extgraph.hh \
   graphmock.hh \
   xmock.hh \
   xwin-mock.hh \
   canvas.hh \
   circle.hh \
   group.hh \
   rectangle.hh \
   shape.hh \
   text.hh \
   xwin.hh

HEADERS += $(PSHDR)

.PHONY: test clean

all: test

test: $(TARGET) test-group-special
	./$(TARGET) -p -a
	./test-group-special -p -a

test-group-special: $(TEST_GROUP_SPECIAL_OBJS)
	$(LINK.cc) $^ $(LDLIBS) -o $@

EXTRA_OBJ_DEPS = Makefile
wrap-canvas.o:: canvas.cc
$(TEST_GROUP_SPECIAL_OBJS):: $(HEADERS) Makefile
wrap-group.o:: group.cc

CLEAN_EXTRA = test-group-special
CLEAN_DIST = canvas.cc
include $(TOPDIR)/common.mak
