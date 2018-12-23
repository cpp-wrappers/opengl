CXXFLAGS=--std=c++17 -I include/opengl
LIBNAME=libopengl-wrapper
SLB=$(LIBNAME).a
libdir=/usr/local/lib
includedir=/usr/local/include

vpath %.cpp src
vpath %.hpp include/opengl

.PHONY: all
all: $(SLB)

%.hpp:
	
%.cpp:

$(SLB): $(SLB)(internal.o) $(SLB)(debug.o)

internal.o: \
internal.cpp \
internal.hpp \
shader.hpp \
program.hpp \
bindable.hpp \
buffer.hpp \
core.hpp \
debug.hpp \
texture.hpp \
vertex_array.hpp \
with_name.hpp
debug.o: debug.cpp debug.hpp

.PHONY: install
install: all
	install $(LIBNAME).a $(libdir)
	cp -r include/opengl $(includedir)

.PHONY: clean
clean:
	rm -rf $(LIBNAME).a internal.o debug.o

