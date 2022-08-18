#include "vega_mesh.hpp"

namespace vega {
    VEGAMesh::VEGAMesh(std::vector <VEGAVertex>& vertices, std::vector <unsigned int>& indices, std::vector <VEGATexture>& textures)
        : vertices(vertices), indices(indices), textures(textures) {

        vao = new VEGAVertexArray();
        vbo = new VEGAVertexBuffer(vertices);
        ibo = new VEGAIndexBuffer(indices);

        vao->link_attrib(vbo, 0, GL_FLOAT, 3, sizeof(VEGAVertex), (void*) (0 * sizeof(float)));
        vao->link_attrib(vbo, 1, GL_FLOAT, 3, sizeof(VEGAVertex), (void*) (3 * sizeof(float)));
        vao->link_attrib(vbo, 2, GL_FLOAT, 3, sizeof(VEGAVertex), (void*) (6 * sizeof(float)));
        vao->link_attrib(vbo, 3, GL_FLOAT, 2, sizeof(VEGAVertex), (void*) (9 * sizeof(float)));
    }

    VEGAMesh::~VEGAMesh() {
        destroy();
    }

    void VEGAMesh::render(VEGAShader& shader, VEGACamera& camera, glm::mat4 matrix) {
        shader.use();
        vao->bind();
        ibo->bind();

        unsigned int diffuse = 0;
        unsigned int specular = 0;

        for (unsigned int i = 0; i < textures.size(); i++) {
            std::string num;
            std::string type = textures[i].get_type();

            if (type == "diffuse")
                num = std::to_string(diffuse++);

            if (type == "specular")
                num = std::to_string(specular++);

            shader.use();
            textures[i].tex_unit(shader.get_uniform_location((type + num).c_str()), i);
            textures[i].bind();
        }

        transform.uniform(shader.get_uniform_location("transform_matrix"));
        camera.handle_matrix(shader.get_uniform_location("camera_matrix"));
        glUniform3f(shader.get_uniform_location("camera_position"), camera.get_pos()->x, camera.get_pos()->y, camera.get_pos()->z);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        ibo->unbind();
        vao->unbind();

        for (unsigned int i = 0; i < textures.size(); i++)
            textures[i].unbind();
    }

    void VEGAMesh::destroy() {
        delete vao;
        delete vbo;
        delete ibo;
    }
}