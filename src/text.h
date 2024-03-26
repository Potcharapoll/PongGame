#ifndef TEXT_H
#define TEXT_H
#include "renderer.h"

typedef struct {
    ivec2s size;
    unsigned int frame_size;
} text_t;

text_t text_init(int frame_size, ivec2s size);
void text_render(text_t self, const char *txt, float scale, float reduce_padding, vec2s position, vec3s color);
#endif
