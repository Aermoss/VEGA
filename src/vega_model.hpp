#pragma once

#include <json/json.hpp>
#include <glm/glm.hpp>

#include "vega_mesh.hpp"
#include "vega_camera.hpp"
#include "vega_shader.hpp"
#include "vega_file.hpp"
#include "vega_vertex.hpp"
#include "vega_texture.hpp"
#include "vega_transform.hpp"

#include <vector>
#include <string>

using json = nlohmann::json;

namespace vega {
    class VEGAModel {
        private:
            const char* file_path;
            std::vector <unsigned char> data;
            json JSON;

            std::vector <glm::vec3> translates_meshes;
            std::vector <glm::quat> rotations_meshes;
            std::vector <glm::vec3> scales_meshes;
            std::vector <glm::mat4> matrices_meshes;

            std::vector <VEGAMesh*> loaded_meshes;
            std::vector <std::string> loaded_texture_names;
            std::vector <VEGATexture> loaded_textures;

            void load_mesh(unsigned int index);
            void traverse_node(unsigned int next_node, glm::mat4 matrix = glm::mat4(1.0f));

            std::vector <unsigned char> get_data();
            std::vector <float> get_floats(json accessor);
            std::vector <unsigned int> get_indices(json accessor);

            std::vector <VEGATexture> get_textures();
            std::vector <VEGAVertex> assemble_vertices(std::vector <glm::vec3> positions, std::vector <glm::vec3> normals, std::vector <glm::vec2> tex_uvs);

            std::vector <glm::vec2> group_floats_vec2(std::vector <float> floats);
            std::vector <glm::vec3> group_floats_vec3(std::vector <float> floats);
            std::vector <glm::vec4> group_floats_vec4(std::vector <float> floats);

        public:
            VEGAModel(const char* file_path);
            ~VEGAModel();

            VEGATransform transform;

            void render(VEGAShader& shader, VEGACamera& camera);
            void destroy();
    };
}