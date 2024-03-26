#version 330 core
out vec4 frag_color;

uniform sampler2D tex;
uniform vec2 uv;
uniform vec2 size;
uniform float frame_size;

in vec2 tex_coord;
in vec3 color;

void main() {
    float x = frame_size / size.x;
    float y = frame_size / size.y;
    frag_color = vec4(color, texture(tex, vec2(tex_coord.x * x, tex_coord.y * y) + vec2(uv.x * x , uv.y * y)).r);
}
