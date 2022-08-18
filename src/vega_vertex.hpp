#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace vega {
    struct VEGAVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 tex_uv;

        VEGAVertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 tex_uv)
            : position(position), normal(normal), color(color), tex_uv(tex_uv) {}

        VEGAVertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color)
            : position(position), normal(normal), color(color) {}

        VEGAVertex(glm::vec3 position, glm::vec3 normal)
            : position(position), normal(normal) {}

        VEGAVertex(glm::vec3 position)
            : position(position) {}

        VEGAVertex() {}
    };
}