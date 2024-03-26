#include "rectangle.h"

#pragma GCC diagnostic ignored "-Wmissing-braces"

void rectangle_init(struct Rectangle *self, vec3s pos, ivec2s size, vec4s color) {
    self->velocity = glms_vec2_zero();
    self->position = pos;
    self->size     = size;

    self->vertices[0] = (vertex_t){ .position = { pos.x,        pos.y,        pos.z }, .color = color, .tex_coord = {0.0, 0.0} };
    self->vertices[1] = (vertex_t){ .position = { pos.x+size.x, pos.y,        pos.z }, .color = color, .tex_coord = {0.0, 0.0} };
    self->vertices[2] = (vertex_t){ .position = { pos.x+size.x, pos.y+size.y, pos.z }, .color = color, .tex_coord = {0.0, 0.0} };
    self->vertices[3] = (vertex_t){ .position = { pos.x,        pos.y+size.y, pos.z }, .color = color, .tex_coord = {0.0, 0.0} };
}

void rectangle_update_vertices(struct Rectangle *self) {
    self->vertices[0].position = (vec3s){ self->position.x,              self->position.y,              self->position.z };
    self->vertices[1].position = (vec3s){ self->position.x+self->size.u, self->position.y,              self->position.z };
    self->vertices[2].position = (vec3s){ self->position.x+self->size.u, self->position.y+self->size.v, self->position.z };
    self->vertices[3].position = (vec3s){ self->position.x,              self->position.y+self->size.v, self->position.z };
}
