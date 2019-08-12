SEARCHDIRS = . .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../..
TOPNAME = $(word 1,$(wildcard $(addsuffix /top.mak,$(SEARCHDIRS))))
include $(TOPNAME)


TARGET = graphics

LIBS = \
    -lcairo \
    -lX11

include $(TOPDIR)/common.mak
