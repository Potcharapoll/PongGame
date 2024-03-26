#include "shader.h"
#include "state.h"
#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glad/glad.h>

static GLint shader_get_uniform(struct Shader self, char *name) {
    if (hash_table_search(self.uniform, name) == HASH_TABLE_NOT_FOUND) {
        GLint location = glGetUniformLocation(self.handle, name);
        hash_table_insert(self.uniform, name, location);
        return location;
    }
    return hash_table_search(self.uniform, name);
}

static GLuint _compile(GLenum type, char *path) {
    FILE *fp;
    long len;
    char *txt;

    fp = fopen(path, "rb");
    if (fp == NULL) {
        printf("cannot open file at %s\n", path);
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    assert(len > 0);
    fseek(fp, 0, SEEK_SET);
    txt = malloc(len);
    assert(txt != NULL);
    fread(txt, 1, len, fp);
    fclose(fp);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar* const *)&txt, (const GLint*)&len);
    glCompileShader(shader);
    
    int check;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &check);
    if(check == GL_FALSE) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        puts(log);
    }
    free(txt);
    return shader;
}

struct Shader shader_load(char *vs_path, char *fs_path) {
    struct Shader shader = {
        .handle = glCreateProgram(),
        .vs = _compile(GL_VERTEX_SHADER, vs_path),
        .fs = _compile(GL_FRAGMENT_SHADER, fs_path),
        .uniform = hash_table_init(100)
    };
    glAttachShader(shader.handle, shader.vs);
    glAttachShader(shader.handle, shader.fs);

    int check;
    glLinkProgram(shader.handle);
    glGetProgramiv(shader.handle, GL_LINK_STATUS, &check);
    if(check == GL_FALSE) {
        char log[512];
        glGetProgramInfoLog(shader.handle, 512, NULL, log);
        puts(log);
    }

    glValidateProgram(shader.handle);
    glGetProgramiv(shader.handle, GL_VALIDATE_STATUS, &check);
    if(check == GL_FALSE) {
        char log[512];
        glGetProgramInfoLog(shader.handle, 512, NULL, log);
        puts(log);
    }
    return shader;
}

void shader_use(struct Shader self) {
    glUseProgram(self.handle);
}

void shader_destroy(struct Shader self) {
    hash_table_destroy(self.uniform);
    glDeleteProgram(self.handle);
    glDeleteShader(self.vs);
    glDeleteShader(self.fs); 
}

void shader_uniform_proj(struct Shader self) {
    glUniformMatrix4fv(shader_get_uniform(self, "proj"), 1, GL_FALSE, (const GLfloat*)state.proj.raw);
}

void shader_uniform_mat4(struct Shader self, char *name, mat4s m) {
    glUniformMatrix4fv(shader_get_uniform(self, name), 1, GL_FALSE, (const GLfloat*)m.raw);
}

void shader_uniform_float(struct Shader self, char *name, float f) {
    glUniform1f(shader_get_uniform(self, name), f);
}

void shader_uniform_vec2(struct Shader self, char *name, vec2s v) {
    glUniform2f(shader_get_uniform(self, name), v.raw[0], v.raw[1]);
}
