#include "text.h"
#include "renderer.h"
#include "shader.h"
#include "vao.h"
#include "state.h"
#include "util.h"

static void update_vertices(vertex_t self[static 4], float x, float y, float size) {
        self[0].position = (vec3s){x,      y,      0.0f};
        self[1].position = (vec3s){x+size, y,      0.0f};
        self[2].position = (vec3s){x+size, y+size, 0.0f};
        self[3].position = (vec3s){x,      y+size, 0.0f};
}

text_t text_init(int frame_size, ivec2s size) {
    text_t text = {
        .size = size,
        .frame_size = frame_size
    };
    return text;
} 

void text_render(text_t self, const char *txt, float scale, float reduce_padding, vec2s position, vec3s color) {
    float size  = self.frame_size*scale;
    int rows    = 0;
    int columns = 0;

    int start_alpha = 4;
    int start_digit = 1;

    float xpos = position.x;
    float ypos = position.y;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderer_texture_bind(&state.renderer, TEXTURE_TEXT);
    renderer_shader_use(&state.renderer, SHADER_TEXT);

    shader_uniform_vec2(state.renderer.shader, "size", (vec2s){(float)self.size.raw[0], (float)self.size.raw[1]});
    shader_uniform_float(state.renderer.shader, "frame_size", self.frame_size);

    vertex_t vertices[4] = {
        { .position = {xpos,      ypos,      0.0f}, .color = {color.x, color.y, color.z, 1.0}, .tex_coord = {0.0, 0.0}},
        { .position = {xpos+size, ypos,      0.0f}, .color = {color.x, color.y, color.z, 1.0}, .tex_coord = {1.0, 0.0}},
        { .position = {xpos+size, ypos+size, 0.0f}, .color = {color.x, color.y, color.z, 1.0}, .tex_coord = {1.0, 1.0}},
        { .position = {xpos,      ypos+size, 0.0f}, .color = {color.x, color.y, color.z, 1.0}, .tex_coord = {0.0, 1.0}}
    };

    for (const char *c = txt; *c; c++) {

        if (isalpha(*c)) {
            rows = *c - 'A';
            columns = start_alpha;
            if (rows > 7) {
                columns -= (rows / 8);
                rows %= 8;
            }
        }

        if(isdigit(*c)) {
            rows = *c - '0' + 2;
            columns = start_digit;
            if (rows > 7) {
                columns -= (rows / 8);
                rows %= 8;
            }
        }
        shader_uniform_vec2(state.renderer.shader, "uv", (vec2s){rows, columns});
        update_vertices(vertices, xpos, ypos, size);
        
        vbo_bind(state.renderer.vbo);
        vbo_subdata(state.renderer.vbo, sizeof(vertices), vertices);

        vao_bind(state.renderer.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

        xpos += (size - self.frame_size*reduce_padding);
    }
    renderer_texture_bind(&state.renderer, TEXTURE_NONE);
}
