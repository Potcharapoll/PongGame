#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

uniform mat4 proj;

out vec4 color;

void main() {
    color = a_color;
    gl_Position = proj * vec4(a_pos, 1.0);
}

