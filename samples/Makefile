SEARCHDIRS = . .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../..
TOPNAME = $(word 1,$(wildcard $(addsuffix /top.mak,$(SEARCHDIRS))))
include $(TOPNAME)

INCLUDES = -I$(TOPDIR)/include

SRCS = \
	pmr \


include $(TOPDIR)/common.mak
