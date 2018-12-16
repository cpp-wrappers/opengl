CXX=clang++
CXXFLAGS=-std=c++17 -I opengl
LIBNAME=libopengl-wrapper
libdir=/usr/local/lib
includedir=/usr/local/include

all: static_lib

.PHONY: static_lib
static_lib: internal.o debug.o
	$(AR) rcs $(LIBNAME).a $^

%.o: %.cpp
	$(COMPILE.cc) $<

.PHONY: install
install: all
	install $(LIBNAME).a $(libdir)
	cp -r opengl $(includedir)

