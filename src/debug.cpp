#include "gl/debug.hpp"

#ifdef GL_ALL_INCLUE_HEADER
    #include GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

void gl::internal::debug_message_callback(gl::debug_callback cb, const void *user_param) {
	glDebugMessageCallback((GLDEBUGPROC)cb, user_param);
}