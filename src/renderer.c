#include "renderer.h"
#include "hashtable.h"
#include "shader.h"
#include "texture.h"
#include "vbo.h"
#include "types.h"
#include "state.h"

#include <glad/glad.h>

void renderer_init(struct Renderer *self) {
    self->vao = vao_create(); 
    self->vbo = vbo_create(GL_ARRAY_BUFFER, true);
    self->ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, false);

    vao_bind(self->vao);

    vbo_bind(self->vbo);
    vbo_data(self->vbo, 4*sizeof(struct Vertex), NULL);
    vao_attr(0, 3, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, position));
    vao_attr(1, 4, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, color));
    vao_attr(2, 2, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, tex_coord));

    u8 indices[] = {0,1,2,2,3,0};
    vbo_bind(self->ebo);
    vbo_data(self->ebo, sizeof(indices), indices);

    vao_unbind();
    vbo_unbind(self->vbo);
    vbo_unbind(self->ebo);

    state.proj = glms_ortho(0, state.window->size.u, 0, state.window->size.v, 0.0, 100.0f);


    self->shaders[SHADER_TEXT]  = shader_load("res/shaders/text.vs", "res/shaders/text.fs");
    renderer_shader_use(self, SHADER_TEXT);
    shader_uniform_proj(self->shader);

    self->shaders[SHADER_BASIC] = shader_load("res/shaders/basic.vs", "res/shaders/basic.fs");
    renderer_shader_use(self, SHADER_BASIC);
    shader_uniform_proj(self->shader);

    self->current_texture = TEXTURE_NONE;
    self->textures[TEXTURE_TEXT] = texture_load("res/images/font.png");
}

void renderer_destroy(struct Renderer *self) {
    for (int i = 0; i < SHADER_COUNT; i++) {
        shader_destroy(self->shaders[i]);
    }

    vao_destroy(self->vao);
    vbo_destroy(self->vbo);
    vbo_destroy(self->ebo);
}

void renderer_clear(void) {
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_shader_use(struct Renderer *self, enum Shaders shader) {
    self->shader = self->shaders[shader];
    shader_use(self->shader);

    self->current_shader = shader;
}

void renderer_texture_bind(struct Renderer *self, enum Textures texture) {
    if (texture == TEXTURE_NONE) {
        texture_unbind();
    }
    else {
        self->texture = self->textures[texture];
        texture_bind(self->texture);
    }

    self->current_texture = texture;
}
