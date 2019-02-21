override CXXFLAGS += --std=c++17
CPPFLAGS = -I include/opengl
LIBNAME=opengl-wrapper
libfile=lib$(LIBNAME).a
libdir=/usr/local/lib
includedir=/usr/local/include

vpath %.cpp src
vpath %.hpp include/opengl

%.hpp:
	
srcs := internal.cpp debug.cpp
objects := $(srcs:.cpp=.o)
deps := $(srcs:.cpp=.d)

$(libfile): $(libfile)($(objects))

%.d : %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(deps)

.PHONY: clean
clean:
	rm -f $(libfile) $(objects) $(deps)

