#include "gl/buffer.hpp"

#ifdef GL_BUFFER_INCLUDE_HEADER
    GL_BUFFER_INCLUDE_HEADER
#elif GL_ALL_INCLUE_HEADER
    GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

inline void gl::internal::gen_buffers(uint n, uint* buffers) {
    glGenBuffers(n, buffers);
}

inline void gl::internal::bind_buffer(uint target, uint buffer) {
    glBindBuffer(target, buffer);
}

inline void gl::internal::delete_buffers(uint n, uint* buffers) {
    glDeleteBuffers(n, buffers);
}

inline void gl::internal::buffer_data(uint target, uint size, const void* data, uint usage) {
    glBufferData(target, size, data, usage);
}

inline void gl::internal::buffer_sub_data(uint target, uint offset, uint size, const void* data) {
    glBufferSubData(target, offset, size, data);
}

inline void gl::internal::get_buffer_parameteriv(uint target, uint pname, int* data) {
    glGetBufferParameteriv(target, pname, data);
}