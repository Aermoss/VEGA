#version 460

out vec4 outColor;

in Data {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
} data;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 cameraPosition;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform float ambient;

vec4 pointLight() {
    vec3 lightVec = lightPosition - data.position;
    float dist = length(lightVec);
    float a = 3.0f;
    float b = 0.7f;
    float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);
    vec3 normal = normalize(data.normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(cameraPosition - data.position);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specularAmount * specularLight;
    return (texture(diffuse0, data.texCoord) * (diffuse * intensity + ambient) + texture(specular0, data.texCoord).r * specular * intensity) * lightColor;
}

vec4 directionalLight() {
    vec3 normal = normalize(data.normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(cameraPosition - data.position);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specularAmount * specularLight;
    return (texture(diffuse0, data.texCoord) * (diffuse + ambient) + texture(specular0, data.texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
    float outerCone = 0.90f;
    float innerCone = 0.95f;
    vec3 normal = normalize(data.normal);
    vec3 lightDirection = normalize(lightPosition - data.position);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(cameraPosition - data.position);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specularAmount * specularLight;
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
    return (texture(diffuse0, data.texCoord) * (diffuse * intensity + ambient) + texture(specular0, data.texCoord).r * specular * intensity) * lightColor;
}

void main() {
    outColor = directionalLight();
}