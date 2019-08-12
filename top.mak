# define the absolute name for the topmost directory of our project/workshop
TOPDIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
