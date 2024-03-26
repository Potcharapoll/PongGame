#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "renderer.h"

struct Rectangle {
    vertex_t vertices[4];
    vec3s position;
    vec2s velocity;
    ivec2s size;
};

void rectangle_init(struct Rectangle *self, vec3s pos, ivec2s size, vec4s color);
void rectangle_update_vertices(struct Rectangle *self);
#endif
