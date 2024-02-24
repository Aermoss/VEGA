#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "vertexArray.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "camera.hpp"
#include "transform.hpp"

#include <vector>

namespace vega {
    class VEGAMesh {
        private:
            std::vector<VEGAVertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<VEGATexture*> textures;

            VEGAVertexArray* vao;
            VEGAVertexBuffer* vbo;
            VEGAIndexBuffer* ibo;
            
        public:
            VEGATransform transform;

            VEGAMesh(std::vector<VEGAVertex>& vertices, std::vector<unsigned int>& indices, std::vector<VEGATexture*>& textures);
            ~VEGAMesh();

            void render(VEGAShader* shader, VEGACamera* camera, glm::mat4 matrix = glm::mat4(1.0f));
            void destroy();
    };
}