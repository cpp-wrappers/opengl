#include "gl/program.hpp"

#ifdef GL_PROGRAM_INCLUDE_HEADER
    GL_PROGRAM_INCLUDE_HEADER
#elif GL_ALL_INCLUE_HEADER
    GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

inline uint gl::internal::create_program() {
    glCreateProgram();
}

inline void gl::internal::delete_program(uint name) {
    glDeleteProgram(name);
}

inline void gl::internal::attach_shader(uint program, uint shader) {
    glAttachShader(program, shader);
}

inline void gl::internal::detach_shader(uint program, uint shader) {
    glDetachShader(program, shader);
}

inline void gl::internal::link_program(uint program) {
    glLinkProgram(program);
}

inline void gl::internal::use_program(uint program) {
    glUseProgram(program);
}

inline int gl::internal::get_uniform_location(uint program, const char *name) {
    glGetUniformLocation(program, name);
}

inline int gl::internal::get_attribute_location(uint program, const char *name) {
    glGetAttribLocation(program, name);
}

inline void gl::internal::draw_arrays(uint mode, int first, uint count) {
    glDrawArrays(mode, first, count);
}

inline void gl::internal::get_program_info_log(uint program, int buf_size, int *length, char *info_log) {
    glGetProgramInfoLog(program, buf_size, length, info_log);
}

inline void gl::internal::get_programiv(uint program, uint pname, int *params ) {
    glGetProgramiv(program, pname, params);
}

inline void gl::internal::uniform(int location, int value) {
    glUniform1i(location, value);
}

inline void gl::internal::uniform(int location, int v1, int v2) {
    glUniform2i(location, v1, v2);
}

inline void gl::internal::uniform(int location, uint value) {
    glUniform1ui(location, value);
}

inline void gl::internal::uniform(int location, float f1) {
    glUniform1f(location, f1);
}

inline void gl::internal::uniform(int location, float f1, float f2) {
    glUniform2f(location, f1, f2);
}

inline void gl::internal::uniform(int location, float f1, float f2, float f3, float f4) {
    glUniform4f(location, f1, f2 ,f3, f4);
}

inline void gl::internal::uniform_1iv(int location, uint count, const int* value) {
    glUniform1iv(location, count, value);
}

inline void gl::internal::uniform_matrix_4fv(int location, uint count, bool transpose, const float* value) {
    glUniformMatrix4fv(location, count, transpose, value);
}