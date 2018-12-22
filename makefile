CXXFLAGS=--std=c++17 -I include/opengl
LIBNAME=libopengl-wrapper
SLB=$(LIBNAME).a
libdir=/usr/local/lib
includedir=/usr/local/include

vpath %.cpp src
vpath %.hpp include/opengl

.PHONY: all
all: $(LB)

%.hpp:
	
%.cpp:

$(SLB): $(SLB)(internal.o) $(SLB)(debug.o)

internal.o: internal.cpp %.hpp
debug.o: debug.cpp debug.hpp

.PHONY: install
install: all
	install $(LIBNAME).a $(libdir)
	cp -r include/opengl $(includedir)

.PHONY: clean
clean:
	rm -rf $(LIBNAME).a internal.o debug.o

