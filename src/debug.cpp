#include "gl/debug.hpp"
#include <functional>

#ifdef GL_ALL_INCLUE_HEADER
    #include GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

thread_local gl::debug_message_callback_t tl_dm_callback;

void gl::debug_message_callback(gl::debug_message_callback_t callback) {
    tl_dm_callback = callback;

    glDebugMessageCallback(
        [](
            GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar *message, const void *userParam
        ) {
            ((gl::debug_message_callback_t*)userParam)->operator()({{message, (unsigned long long)length}});
        },
        &tl_dm_callback
    );
}