CXXFLAGS = -std=c++14

ENVCXX := $(shell echo "$$CXX")
ifeq ($(ENVCXX),)
CXX = g++
endif

ifeq ((patsubst clang%,,$(CXX)), clang)
#CXX = clang++
CXXFLAGS += -stdlib=libc++
endif

TARGETS = \
	chrono \
	hash-set \
	init-list \
	lambda \
	move \
	nothrow-specialize \
	regex \
	static-vector-simple-move \
	unique-ptr \
	variadic-templ

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *~
