#include "gl/core.hpp"

#ifdef GL_CORE_INCLUDE_HEADER
    GL_CORE_INCLUDE_HEADER
#elif GL_ALL_INCLUE_HEADER
    GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

inline void gl::internal::clear(uint mask) {
    glClear(mask);
}

inline void gl::internal::clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

inline void gl::internal::viewport(int x, int y, uint w, uint h) {
    glViewport(x, y, w, h);
}

inline void gl::internal::enable(uint name) {
    glEnable(name);
}

inline void gl::internal::blend_func(uint source, uint destination) {
    glBlendFunc(source, destination);
}
