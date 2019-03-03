#include "gl/vertex_array.hpp"

#ifdef GL_VERTEX_ARRAY_INCLUDE_HEADER
    GL_VERTEX_ARRAY_INCLUDE_HEADER
#elif GL_ALL_INCLUE_HEADER
    GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

inline void gl::internal::gen_vertex_arrays(uint n, uint* arrays) {
    glGenVertexArrays(n, arrays);
}

inline void gl::internal::bind_vertex_array(uint array) {
    glBindVertexArray(array);
}

inline void gl::internal::delete_vertex_arrays(uint n, uint* arrays) {
    glDeleteVertexArrays(n, arrays);
}

inline void gl::internal::vertex_attrib_pointer(uint index, int size, uint type, bool normalized, uint stride, const void* pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
inline void gl::internal::vertex_attrib_i_pointer(uint index, int size, uint type, uint stride, const void *pointer) {
    glVertexAttribIPointer(index, size, type, stride, pointer);
}

inline void gl::internal::bind_vertex_buffer(uint bindingindex, uint buffer, uint offset, uint stride) {
    glBindVertexBuffer(bindingindex, buffer, offset, stride);
}

inline void gl::internal::enable_vertex_attrib_array(uint index) {
    glEnableVertexAttribArray(index);
}

inline void gl::internal::get_vertex_attribiv(uint index, uint pname, int *params) {
    glGetVertexAttribiv(index, pname, params);
}