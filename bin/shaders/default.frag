#version 330 core

in vec3 current_position;
in vec3 frag_normal;
in vec3 frag_color;
in vec2 frag_tex_uv;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 camera_position;

uniform vec4 light_color;
uniform vec3 light_position;
uniform float ambient;

vec4 point_light() {
    vec3 light_vec = light_position - current_position;
    float dist = length(light_vec);
    float a = 3.0f;
    float b = 0.7f;
    float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 normal = normalize(frag_normal);
    vec3 light_direction = normalize(light_vec);
    float diffuse = max(dot(normal, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_direction = normalize(camera_position - current_position);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
    float specular = specular_amount * specular_light;

    return (texture(diffuse0, frag_tex_uv) * (diffuse * intensity + ambient) + texture(specular0, frag_tex_uv).r * specular * intensity) * light_color;
}

vec4 directional_light() {
    vec3 normal = normalize(frag_normal);
    vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_direction = normalize(camera_position - current_position);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
    float specular = specular_amount * specular_light;

    return (texture(diffuse0, frag_tex_uv) * (diffuse + ambient) + texture(specular0, frag_tex_uv).r * specular) * light_color;
}

vec4 spot_light() {
    float outer_cone = 0.90f;
    float inner_cone = 0.95f;

    vec3 normal = normalize(frag_normal);
    vec3 light_direction = normalize(light_position - current_position);
    float diffuse = max(dot(normal, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_direction = normalize(camera_position - current_position);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
    float specular = specular_amount * specular_light;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
    float intensity = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f);

    return (texture(diffuse0, frag_tex_uv) * (diffuse * intensity + ambient) + texture(specular0, frag_tex_uv).r * specular * intensity) * light_color;
}

void main() {
    gl_FragColor = directional_light();
}