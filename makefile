CXXFLAGS = --std=c++17
CPPFLAGS = -I include/opengl
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

srcs := internal.cpp debug.cpp
objects := $(srcs:.cpp=.o)

%.d : %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(srcs:.cpp=.d)

$(SLB): $(SLB)($(objects))

.PHONY: install
install: all
	install $(LIBNAME).a $(libdir)
	cp -r include/opengl $(includedir)

.PHONY: clean
clean:
	rm -rf $(LIBNAME).a $(objects)

