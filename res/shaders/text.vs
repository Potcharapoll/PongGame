#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex;

uniform mat4 proj;

out vec3 color;
out vec2 tex_coord;

void main() {
    color = a_color.xyz;
    tex_coord = a_tex;
    gl_Position = proj * vec4(a_pos, 1.0);
}
