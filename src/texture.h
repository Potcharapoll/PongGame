#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <cglm/struct.h>

struct Texture {
    GLuint handle;
    ivec2s size;
};

struct Texture texture_load(const char *path);
void texture_destroy(struct Texture self);
void texture_bind(struct Texture self);
void texture_unbind(void);
#endif
