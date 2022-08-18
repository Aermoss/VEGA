#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "vega_vertex_array.hpp"
#include "vega_vertex_buffer.hpp"
#include "vega_index_buffer.hpp"
#include "vega_texture.hpp"
#include "vega_shader.hpp"
#include "vega_vertex.hpp"
#include "vega_camera.hpp"
#include "vega_transform.hpp"

#include <vector>

namespace vega {
    class VEGAMesh {
        private:
            std::vector <VEGAVertex> vertices;
            std::vector <unsigned int> indices;
            std::vector <VEGATexture> textures;

            VEGAVertexArray* vao;
            VEGAVertexBuffer* vbo;
            VEGAIndexBuffer* ibo;
            
        public:
            VEGAMesh(std::vector <VEGAVertex>& vertices, std::vector <unsigned int>& indices, std::vector <VEGATexture>& textures);
            ~VEGAMesh();

            VEGATransform transform;

            void render(VEGAShader& shader, VEGACamera& camera, glm::mat4 matrix = glm::mat4(1.0f));
            void destroy();
    };
}