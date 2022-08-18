#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 tex_uv;

out vec3 current_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_tex_uv;

uniform mat4 camera_matrix;
uniform mat4 transform_matrix;

void main() {
    current_position = vec3(transform_matrix * vec4(position, 1.0f));
    frag_normal = normal;
    frag_color = color;
    frag_tex_uv = mat2(0.0f, -1.0f, 1.0f, 0.0f) * tex_uv;
    gl_Position = camera_matrix * vec4(current_position, 1.0f);
}