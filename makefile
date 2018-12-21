CXXFLAGS=--std=c++17 -I include/opengl
LIBNAME=libopengl-wrapper
libdir=/usr/local/lib
includedir=/usr/local/include

vpath %.cpp src
vpath %.hpp include/opengl

.PHONY: all
all: $(LIBNAME).a

%.hpp:
	
%.cpp:

$(LIBNAME).a: $(LIBNAME).a(internal.o) $(LIBNAME).a(debug.o)

internal.o: internal.cpp %.hpp
debug.o: debug.cpp debug.hpp

.PHONY: install
install: all
	install $(LIBNAME).a $(libdir)
	cp -r include/opengl $(includedir)

.PHONY: clean
clean:
	rm -rf $(LIBNAME).a internal.o debug.o

