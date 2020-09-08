#include "../include/gl/draw.hpp"

using namespace gl;

#ifdef GL_ALL_INCLUE_HEADER
    #include GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/gl.h"
    #include "GL/glcorearb.h"
#endif

void gl::internal::begin(unsigned mode) { glBegin(mode); }
void gl::internal::end() { glEnd(); }

void gl::internal::vertex(float x, float y) { glVertex2f(x, y); }
void gl::internal::normal(float x, float y, float z) { glNormal3f(x, y, z); }