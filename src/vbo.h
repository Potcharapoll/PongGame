#ifndef VBO_H
#define VBO_H
#include <glad/glad.h>
#include <stdio.h>

struct VBO {
    GLuint handle;
    GLenum type;
    bool dynamic;
};

struct VBO vbo_create(GLenum type, bool dynamic);
void vbo_destroy(struct VBO self);
void vbo_bind(struct VBO self);
void vbo_unbind(struct VBO self);
void vbo_data(struct VBO self, size_t size, void *data);
void vbo_subdata(struct VBO self, size_t size, void *data);
#endif
