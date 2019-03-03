#include "gl/debug.hpp"

#ifdef GL_DEBUG_INCLUDE_HEADER
    GL_DEBUG_INCLUDE_HEADER
#elif GL_ALL_INCLUE_HEADER
    GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

inline void gl::internal::debug_message_callback(gl::debug_callback cb, const void *user_param) {
	glDebugMessageCallback((GLDEBUGPROC)cb, user_param);
}