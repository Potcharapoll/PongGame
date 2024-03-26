#ifndef RENDERER_H
#define RENDERER_H
#include "vao.h"
#include "vbo.h"
#include "shader.h"
#include "texture.h"

enum Textures {
    TEXTURE_TEXT,
    TEXTURE_COUNT,

    TEXTURE_NONE
};

enum Shaders {
    SHADER_BASIC,
    SHADER_TEXT,
    SHADER_COUNT
};

typedef struct Vertex {
    vec3s position;
    vec4s color;
    vec2s tex_coord;
} vertex_t;

struct Renderer {
    struct VAO vao;
    struct VBO vbo, ebo;

    enum Shaders current_shader;
    struct Shader shaders[SHADER_COUNT];
    struct Shader shader;


    enum Textures current_texture;
    struct Texture textures[TEXTURE_COUNT];
    struct Texture texture;
};

void renderer_init(struct Renderer *self);
void renderer_clear(void);
void renderer_destroy(struct Renderer *self);
void renderer_shader_use(struct Renderer *self, enum Shaders shader);
void renderer_texture_bind(struct Renderer *self, enum Textures texture);
#endif
