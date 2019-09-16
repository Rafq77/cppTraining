SEARCHDIRS = . .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../..
TOPNAME = $(word 1,$(wildcard $(addsuffix /top.mak,$(SEARCHDIRS))))
include $(TOPNAME)

INCLUDES = -I$(TOPDIR)/include

TARGET = staticVect

all: $(TARGET)

main.o:: staticVector.tt

include $(TOPDIR)/common.mak
