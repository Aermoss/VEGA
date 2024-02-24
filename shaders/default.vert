#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out Data {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
} data;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
    data.position = vec3(model * vec4(position, 1.0f));
    data.normal = normal;
    data.color = color;
    data.texCoord = mat2(0.0f, -1.0f, 1.0f, 0.0f) * texCoord;
    gl_Position = proj * view * vec4(data.position, 1.0f);
}