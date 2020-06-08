#include "gl/program.hpp"

#ifdef GL_PROGRAM_INCLUDE_HEADER
    GL_PROGRAM_INCLUDE_HEADER
#elif GL_ALL_INCLUE_HEADER
    GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

uint gl::internal::create_program() {
    return glCreateProgram();
}

void gl::internal::delete_program(uint name) {
    glDeleteProgram(name);
}

void gl::internal::attach_shader(uint program, uint shader) {
    glAttachShader(program, shader);
}

void gl::internal::detach_shader(uint program, uint shader) {
    glDetachShader(program, shader);
}

void gl::internal::link_program(uint program) {
    glLinkProgram(program);
}

void gl::internal::use_program(uint program) {
    glUseProgram(program);
}

int gl::internal::get_uniform_location(uint program, const char *name) {
    return glGetUniformLocation(program, name);
}

int gl::internal::get_attribute_location(uint program, const char *name) {
    return glGetAttribLocation(program, name);
}

void gl::internal::draw_arrays(uint mode, int first, uint count) {
    glDrawArrays(mode, first, count);
}

void gl::internal::get_program_info_log(uint program, int buf_size, int *length, char *info_log) {
    glGetProgramInfoLog(program, buf_size, length, info_log);
}

void gl::internal::get_programiv(uint program, uint pname, int *params ) {
    glGetProgramiv(program, pname, params);
}

void gl::internal::uniform(int location, int value) {
    glUniform1i(location, value);
}

void gl::internal::uniform(int location, int v1, int v2) {
    glUniform2i(location, v1, v2);
}

void gl::internal::uniform(int location, uint value) {
    glUniform1ui(location, value);
}

void gl::internal::uniform(int location, float f1) {
    glUniform1f(location, f1);
}

void gl::internal::uniform(int location, float f1, float f2) {
    glUniform2f(location, f1, f2);
}

void gl::internal::uniform(int location, float f1, float f2, float f3, float f4) {
    glUniform4f(location, f1, f2 ,f3, f4);
}

void gl::internal::uniform_1iv(int location, uint count, const int* value) {
    glUniform1iv(location, count, value);
}

void gl::internal::uniform_matrix_4fv(int location, uint count, bool transpose, const float* value) {
    glUniformMatrix4fv(location, count, transpose, value);
}