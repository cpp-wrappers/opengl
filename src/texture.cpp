#include "gl/texture.hpp"

#ifdef GL_ALL_INCLUE_HEADER
    #include GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glcorearb.h"
#endif

void gl::internal::gen_textures(uint n, uint* textures) {
    glGenTextures(n, textures);
}

void gl::internal::bind_texture(uint target, uint texture) {
    glBindTexture(target, texture);
}

void gl::internal::delete_textures(uint n, uint* textures) {
    glDeleteTextures(n, textures);
}

void gl::internal::texture_parameteri(uint texture, uint pname, int param) {
    glTextureParameteri(texture, pname, param);
}

void gl::internal::tex_parameteri(uint target, uint pname, int param) {
    glTexParameteri(target, pname, param);
}

void gl::internal::get_texture_level_parameteriv(uint texture, int level, uint pname, int* params) {
    glGetTextureLevelParameteriv(texture, level, pname, params);
}

void gl::internal::get_tex_level_parameteriv(uint target, int level, uint pname, int* params) {
    glGetTexLevelParameteriv(target, level, pname, params);
}

void gl::internal::tex_image_2d(uint target, int level, int internalformat,
    uint width, uint height, int border, uint format, uint type, const void* data) {
    glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
}

void gl::internal::tex_sub_image_2d(uint target, int level, int xoffset,
    int yoffset, uint width, uint height, uint format,
    uint type, const void *data) {
    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, data);
}

void gl::internal::active_texture(uint texture) {
    glActiveTexture(texture);
}