#include <vega/mesh.hpp>

namespace vega {
    VEGAMesh::VEGAMesh(std::vector <VEGAVertex>& vertices, std::vector <unsigned int>& indices, std::vector <VEGATexture*>& textures)
        : vertices(vertices), indices(indices), textures(textures) {

        vao = new VEGAVertexArray();
        vbo = new VEGAVertexBuffer(vertices);
        ibo = new VEGAIndexBuffer(indices);

        vao->linkAttrib(vbo, 0, GL_FLOAT, 3, sizeof(VEGAVertex), (void*) (0 * sizeof(float)));
        vao->linkAttrib(vbo, 1, GL_FLOAT, 3, sizeof(VEGAVertex), (void*) (3 * sizeof(float)));
        vao->linkAttrib(vbo, 2, GL_FLOAT, 3, sizeof(VEGAVertex), (void*) (6 * sizeof(float)));
        vao->linkAttrib(vbo, 3, GL_FLOAT, 2, sizeof(VEGAVertex), (void*) (9 * sizeof(float)));
    }

    VEGAMesh::~VEGAMesh() {
        
    }

    void VEGAMesh::render(VEGAShader* shader, VEGACamera* camera, glm::mat4 matrix) {
        shader->use();
        vao->bind();
        ibo->bind();

        uint32_t diffuse = 0;
        uint32_t specular = 0;

        for (uint32_t i = 0; i < textures.size(); i++) {
            std::string num;
            std::string type = textures[i]->getType();

            if (type == "diffuse")
                num = std::to_string(diffuse++);

            if (type == "specular")
                num = std::to_string(specular++);

            textures[i]->texUnit(shader->getUniformLocation((type + num).c_str()), i);
            textures[i]->bind();
        }

        camera->updateMatrices(shader);
        transform.uniform(shader->getUniformLocation("model"));
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        for (uint32_t i = 0; i < textures.size(); i++)
            textures[i]->unbind();

        ibo->unbind();
        vao->unbind();
        shader->unuse();
    }

    void VEGAMesh::destroy() {
        vao->destroy();
        vbo->destroy();
        ibo->destroy();
        delete vao;
        delete vbo;
        delete ibo;
    }
}