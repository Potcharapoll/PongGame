#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include "hashtable.h"

#include <cglm/struct.h>
#include <cglm/cglm.h>

struct Shader {
    GLuint handle, vs, fs;
    hash_table_t *uniform;
};

struct Shader shader_load(char *vs_path, char *fs_path);
void shader_use(struct Shader self);
void shader_destroy(struct Shader self);
void shader_uniform_proj(struct Shader self);
void shader_uniform_mat4(struct Shader self, char *name, mat4s m);
void shader_uniform_float(struct Shader self, char *name, float f);
void shader_uniform_vec2(struct Shader self, char *name, vec2s v);
#endif
