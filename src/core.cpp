#include "gl/core.hpp"

#ifdef GL_ALL_INCLUE_HEADER
    #include GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

void gl::internal::clear(uint mask) {
    glClear(mask);
}

void gl::internal::clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void gl::internal::viewport(int x, int y, uint w, uint h) {
    glViewport(x, y, w, h);
}

void gl::internal::enable(uint name) {
    glEnable(name);
}

void gl::internal::blend_func(uint source, uint destination) {
    glBlendFunc(source, destination);
}