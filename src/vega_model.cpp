#include "vega_model.hpp"

namespace vega {
    VEGAModel::VEGAModel(const char* file_path) : file_path(file_path) {
        std::string text = VEGAReadFile(file_path);
        JSON = json::parse(text);
        data = get_data();

        traverse_node(0);
    }

    VEGAModel::~VEGAModel() {
        destroy();
    }

    void VEGAModel::render(VEGAShader& shader, VEGACamera& camera) {
        for (unsigned int i = 0; i < loaded_meshes.size(); i++) {
            loaded_meshes[i]->transform.set(transform.get());
            loaded_meshes[i]->render(shader, camera, matrices_meshes[i]);
        }
    }

    void VEGAModel::load_mesh(unsigned int index) {
        unsigned int positions_index = JSON["meshes"][index]["primitives"][0]["attributes"]["POSITION"];
        unsigned int normal_index = JSON["meshes"][index]["primitives"][0]["attributes"]["NORMAL"];
        unsigned int tex_uv_index = JSON["meshes"][index]["primitives"][0]["attributes"]["TEXCOORD_0"];
        unsigned int indices_index = JSON["meshes"][index]["primitives"][0]["indices"];

        std::vector <float> position_floats =  get_floats(JSON["accessors"][positions_index]);
        std::vector <glm::vec3> positions = group_floats_vec3(position_floats);
        std::vector <float> normal_floats =  get_floats(JSON["accessors"][normal_index]);
        std::vector <glm::vec3> normals = group_floats_vec3(normal_floats);
        std::vector <float> tex_uv_floats =  get_floats(JSON["accessors"][tex_uv_index]);
        std::vector <glm::vec2> tex_uvs = group_floats_vec2(tex_uv_floats);

        std::vector <VEGAVertex> vertices = assemble_vertices(positions, normals, tex_uvs);
        std::vector <unsigned int> indices =  get_indices(JSON["accessors"][indices_index]);
        std::vector <VEGATexture> textures = get_textures();

        loaded_meshes.push_back(new VEGAMesh(vertices, indices, textures));
    }

    void VEGAModel::traverse_node(unsigned int next_node, glm::mat4 matrix) {
        json node = JSON["nodes"][next_node];

        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

        if (node.find("translation") != node.end()) {
            float translation_values[3];

            for (unsigned int i = 0; i < node["translation"].size(); i++)
                translation_values[i] = (node["translation"][i]);

            translation = glm::make_vec3(translation_values);
        }

        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        if (node.find("rotation") != node.end()) {
            float rotation_values[4] = {node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2]};

            rotation = glm::make_quat(rotation_values);
        } 

        glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);

        if (node.find("scale") != node.end()) {
            float scale_values[3];

            for (unsigned int i = 0; i < node["scale"].size(); i++)
                scale_values[i] = (node["scale"][i]);

            scale = glm::make_vec3(scale_values);
        }

        glm::mat4 matrix_node = glm::mat4(1.0f);

        if (node.find("matrix") != node.end()) {
            float matrix_values[16];

            for (unsigned int i = 0; i < node["matrix"].size(); i++)
                matrix_values[i] = (node["matrix"][i]);

            matrix_node = glm::make_mat4(matrix_values);
        }

        glm::mat4 translation_matrix = glm::mat4(1.0f);
        glm::mat4 rotation_matrix = glm::mat4(1.0f);
        glm::mat4 scale_matrix = glm::mat4(1.0f);

        translation_matrix = glm::translate(translation_matrix, translation);
        rotation_matrix = glm::mat4_cast(rotation);
        scale_matrix = glm::scale(scale);

        glm::mat4 matrix_next_node = matrix * matrix_node * translation_matrix * rotation_matrix * scale_matrix;

        if (node.find("mesh") != node.end()) {
            translates_meshes.push_back(translation);
            rotations_meshes.push_back(rotation);
            scales_meshes.push_back(scale);
            matrices_meshes.push_back(matrix_next_node);

            load_mesh(node["mesh"]);
        }

        if (node.find("children") != node.end()) {
            for (unsigned int i = 0; i < node["children"].size(); i++)
                traverse_node(node["children"][i], matrix_next_node);
        }
    }

    std::vector <unsigned char> VEGAModel::get_data() {
        std::string uri = JSON["buffers"][0]["uri"];
        std::string file_dir = std::string(file_path).substr(0, std::string(file_path).find_last_of("/") + 1);
        std::string bytes_text = VEGAReadFile((file_dir + uri).c_str());

        std::vector <unsigned char> data(bytes_text.begin(), bytes_text.end());
        return data;
    }

    std::vector <float> VEGAModel::get_floats(json accessor) {
        std::vector <float> floats;

        unsigned int buffer_view_index = accessor.value("bufferView", 1);
        unsigned int count = accessor["count"];
        unsigned int accessor_byte_offset = accessor.value("byteOffset", 0);
        std::string type = accessor["type"];

        json buffer_view = JSON["bufferViews"][buffer_view_index];
        unsigned int byte_offset = buffer_view["byteOffset"];

        unsigned int number_per_vertex;

        if (type == "SCALAR")
            number_per_vertex = 1;

        else if (type == "VEC2")
            number_per_vertex = 2;

        else if (type == "VEC3")
            number_per_vertex = 3;

        else if (type == "VEC4")
            number_per_vertex = 4;

        else
            throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3 or VEC4)");

        unsigned int data_begin = byte_offset + accessor_byte_offset;
        unsigned int data_length = count * 4 * number_per_vertex;

        for (unsigned int i = data_begin; i < data_begin + data_length; i) {
            unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
            float value;
            std::memcpy(&value, bytes, sizeof(float));
            floats.push_back(value);
        }

        return floats;
    }

    std::vector <unsigned int> VEGAModel::get_indices(json accessor) {
        std::vector <unsigned int> indices;

        unsigned int buffer_view_index = accessor.value("bufferView", 1);
        unsigned int count = accessor["count"];
        unsigned int accessor_byte_offset = accessor.value("byteOffset", 0);
        unsigned int component_type = accessor["componentType"];

        json buffer_view = JSON["bufferViews"][buffer_view_index];
        unsigned int byte_offset = buffer_view["byteOffset"];

        unsigned int data_begin = byte_offset + accessor_byte_offset;

        if (component_type == 5125) {
            for (unsigned int i = data_begin; i < data_begin + count * 4; i) {
                unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(unsigned int));
                indices.push_back((unsigned int) value);
            }
        }

        else if (component_type == 5123) {
            for (unsigned int i = data_begin; i < data_begin + count * 2; i) {
                unsigned char bytes[] = {data[i++], data[i++]};
                unsigned short value;
                std::memcpy(&value, bytes, sizeof(unsigned short));
                indices.push_back((unsigned int) value);
            }
        }

        else if (component_type == 5122) {
            for (unsigned int i = data_begin; i < data_begin + count * 2; i) {
                unsigned char bytes[] = {data[i++], data[i++]};
                short value;
                std::memcpy(&value, bytes, sizeof(short));
                indices.push_back((unsigned int) value);
            }
        }

        return indices;
    }

    std::vector <VEGATexture> VEGAModel::get_textures() {
        std::vector <VEGATexture> textures;

        std::string file_dir = std::string(file_path).substr(0, std::string(file_path).find_last_of("/") + 1);

        for (unsigned int i = 0; i < JSON["images"].size(); i++) {
            std::string tex_path = JSON["images"][i]["uri"];

            bool skip = false;

            for (unsigned int j = 0; j < loaded_texture_names.size(); j++) {
                if (tex_path == loaded_texture_names[j]) {
                    textures.push_back(loaded_textures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                if (tex_path.find("baseColor") != std::string::npos || tex_path.find("diffuse") != std::string::npos) {
                    VEGATexture texture((file_dir + tex_path).c_str(), "diffuse", loaded_textures.size());
                    textures.push_back(texture);
                    loaded_textures.push_back(texture);
                    loaded_texture_names.push_back(tex_path);
                }

                else if (tex_path.find("metallicRoughness") != std::string::npos || tex_path.find("specular") != std::string::npos) {
                    VEGATexture texture((file_dir + tex_path).c_str(), "specular", loaded_textures.size());
                    textures.push_back(texture);
                    loaded_textures.push_back(texture);
                    loaded_texture_names.push_back(tex_path);
                }
            }
        }

        return textures;
    }

    std::vector <VEGAVertex> VEGAModel::assemble_vertices(std::vector <glm::vec3> positions, std::vector <glm::vec3> normals, std::vector <glm::vec2> tex_uvs) {
        std::vector <VEGAVertex> vertices;

        for (unsigned int i = 0; i < positions.size(); i++) {
            vertices.push_back(VEGAVertex(positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), tex_uvs[i]));
        }

        return vertices;
    }

    std::vector <glm::vec2> VEGAModel::group_floats_vec2(std::vector <float> floats) {
        std::vector <glm::vec2> vectors;

        for (unsigned int i = 0; i < floats.size(); i) {
            vectors.push_back(glm::vec2(floats[i++], floats[i++]));
        }

        return vectors;
    }

    std::vector <glm::vec3> VEGAModel::group_floats_vec3(std::vector <float> floats) {
        std::vector <glm::vec3> vectors;

        for (unsigned int i = 0; i < floats.size(); i) {
            vectors.push_back(glm::vec3(floats[i++], floats[i++], floats[i++]));
        }

        return vectors;
    }

    std::vector <glm::vec4> VEGAModel::group_floats_vec4(std::vector <float> floats) {
        std::vector <glm::vec4> vectors;

        for (unsigned int i = 0; i < floats.size(); i) {
            vectors.push_back(glm::vec4(floats[i++], floats[i++], floats[i++], floats[i++]));
        }

        return vectors;
    }

    void VEGAModel::destroy() {
        for (unsigned int i = 0; i < loaded_meshes.size(); i++) {
            loaded_meshes[i]->destroy();
        }
    }
}