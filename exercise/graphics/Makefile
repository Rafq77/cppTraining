SEARCHDIRS = . .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../..
TOPNAME = $(word 1,$(wildcard $(addsuffix /top.mak,$(SEARCHDIRS))))
include $(TOPNAME)


TARGET = graphics

M4_SOURCES = canvas.cc \
	     demo.cc

SOURCES = $(M4_SOURCES)

LIBS = \
    -lcairo \
    -lX11

CLEAN_DIST = $(M4_SOURCES)

include $(TOPDIR)/common.mak
