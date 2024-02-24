#include <vega/model.hpp>

namespace vega {
    VEGAModel::VEGAModel(const char* filePath) : filePath(filePath) {
        std::string text = VEGAReadFile(filePath);
        JSON = json::parse(text);
        data = getData();
        traverseNode(0);
    }

    VEGAModel::~VEGAModel() {

    }

    void VEGAModel::render(VEGAShader* shader, VEGACamera* camera) {
        for (uint32_t i = 0; i < loadedMeshes.size(); i++) {
            loadedMeshes[i]->transform.set(transform.get());
            loadedMeshes[i]->render(shader, camera, matrices[i]);
        }
    }

    void VEGAModel::loadMesh(uint32_t index) {
        uint32_t positionIndex = JSON["meshes"][index]["primitives"][0]["attributes"]["POSITION"];
        uint32_t normalIndex = JSON["meshes"][index]["primitives"][0]["attributes"]["NORMAL"];
        uint32_t texCoordIndex = JSON["meshes"][index]["primitives"][0]["attributes"]["TEXCOORD_0"];
        uint32_t indicesIndex = JSON["meshes"][index]["primitives"][0]["indices"];

        std::vector<float> positionFloats = getFloats(JSON["accessors"][positionIndex]);
        std::vector<glm::vec3> positions = groupFloatsVec3(positionFloats);
        std::vector<float> normalFloats = getFloats(JSON["accessors"][normalIndex]);
        std::vector<glm::vec3> normals = groupFloatsVec3(normalFloats);
        std::vector<float> texCoordFloats = getFloats(JSON["accessors"][texCoordIndex]);
        std::vector<glm::vec2> texCoords = groupFloatsVec2(texCoordFloats);

        std::vector<VEGAVertex> vertices = assembleVertices(positions, normals, texCoords);
        std::vector<uint32_t> indices = getIndices(JSON["accessors"][indicesIndex]);
        std::vector<VEGATexture*> textures = getTextures();

        loadedMeshes.push_back(new VEGAMesh(vertices, indices, textures));
    }

    void VEGAModel::traverseNode(uint32_t nextNode, glm::mat4 matrix) {
        json node = JSON["nodes"][nextNode];

        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

        if (node.find("translation") != node.end()) {
            float translationValues[3];

            for (uint32_t i = 0; i < node["translation"].size(); i++)
                translationValues[i] = (node["translation"][i]);

            translation = glm::make_vec3(translationValues);
        }

        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        if (node.find("rotation") != node.end()) {
            float rotationValues[4] = {node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2]};
            rotation = glm::make_quat(rotationValues);
        } 

        glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);

        if (node.find("scale") != node.end()) {
            float scaleValues[3];

            for (uint32_t i = 0; i < node["scale"].size(); i++)
                scaleValues[i] = (node["scale"][i]);

            scale = glm::make_vec3(scaleValues);
        }

        glm::mat4 matrixNode = glm::mat4(1.0f);

        if (node.find("matrix") != node.end()) {
            float matrixValues[16];

            for (uint32_t i = 0; i < node["matrix"].size(); i++)
                matrixValues[i] = (node["matrix"][i]);

            matrixNode = glm::make_mat4(matrixValues);
        }

        glm::mat4 translationMatrix = glm::mat4(1.0f);
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        glm::mat4 scaleMatrix = glm::mat4(1.0f);

        translationMatrix = glm::translate(translationMatrix, translation);
        rotationMatrix = glm::mat4_cast(rotation);
        scaleMatrix = glm::scale(scale);

        glm::mat4 matrixNextNode = matrix * matrixNode * translationMatrix * rotationMatrix * scaleMatrix;

        if (node.find("mesh") != node.end()) {
            translations.push_back(translation);
            rotations.push_back(rotation);
            scales.push_back(scale);
            matrices.push_back(matrixNextNode);
            loadMesh(node["mesh"]);
        }

        if (node.find("children") != node.end()) {
            for (uint32_t i = 0; i < node["children"].size(); i++)
                traverseNode(node["children"][i], matrixNextNode);
        }
    }

    std::vector<unsigned char> VEGAModel::getData() {
        std::string uri = JSON["buffers"][0]["uri"];
        std::string fileDir = std::string(filePath).substr(0, std::string(filePath).find_last_of("/") + 1);
        std::string bytesText = VEGAReadFile((fileDir + uri).c_str());
        std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
        return data;
    }

    std::vector<float> VEGAModel::getFloats(json accessor) {
        std::vector<float> floats;

        uint32_t bufferViewIndex = accessor.value("bufferView", 1);
        uint32_t count = accessor["count"];
        uint32_t accessorByteOffset = accessor.value("byteOffset", 0);
        std::string type = accessor["type"];

        json bufferView = JSON["bufferViews"][bufferViewIndex];
        uint32_t byteOffset = bufferView["byteOffset"];

        uint32_t numberPerVertex;

        if (type == "SCALAR")
            numberPerVertex = 1;

        else if (type == "VEC2")
            numberPerVertex = 2;

        else if (type == "VEC3")
            numberPerVertex = 3;

        else if (type == "VEC4")
            numberPerVertex = 4;

        else
            throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3 or VEC4)");

        uint32_t dataBegin = byteOffset + accessorByteOffset;
        uint32_t dataLength = count * 4 * numberPerVertex;

        for (uint32_t i = dataBegin; i < dataBegin + dataLength; i) {
            unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
            float value;
            std::memcpy(&value, bytes, sizeof(float));
            floats.push_back(value);
        }

        return floats;
    }

    std::vector<uint32_t> VEGAModel::getIndices(json accessor) {
        std::vector<uint32_t> indices;

        uint32_t bufferViewIndex = accessor.value("bufferView", 1);
        uint32_t count = accessor["count"];
        uint32_t accessorByteOffset = accessor.value("byteOffset", 0);
        uint32_t componentType = accessor["componentType"];

        json buffer_view = JSON["bufferViews"][bufferViewIndex];
        uint32_t byteOffset = buffer_view["byteOffset"];

        uint32_t dataBegin = byteOffset + accessorByteOffset;

        if (componentType == 5125) {
            for (uint32_t i = dataBegin; i < dataBegin + count * 4; i) {
                unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
                uint32_t value;
                std::memcpy(&value, bytes, sizeof(uint32_t));
                indices.push_back((uint32_t) value);
            }
        }

        else if (componentType == 5123) {
            for (uint32_t i = dataBegin; i < dataBegin + count * 2; i) {
                unsigned char bytes[] = {data[i++], data[i++]};
                unsigned short value;
                std::memcpy(&value, bytes, sizeof(unsigned short));
                indices.push_back((uint32_t) value);
            }
        }

        else if (componentType == 5122) {
            for (uint32_t i = dataBegin; i < dataBegin + count * 2; i) {
                unsigned char bytes[] = {data[i++], data[i++]};
                short value;
                std::memcpy(&value, bytes, sizeof(short));
                indices.push_back((uint32_t) value);
            }
        }

        return indices;
    }

    std::vector<VEGATexture*> VEGAModel::getTextures() {
        std::vector<VEGATexture*> textures;
        std::string fileDir = std::string(filePath).substr(0, std::string(filePath).find_last_of("/") + 1);

        for (uint32_t i = 0; i < JSON["images"].size(); i++) {
            std::string texPath = JSON["images"][i]["uri"];
            bool skip = false;

            for (uint32_t j = 0; j < loadedTextureNames.size(); j++) {
                if (texPath == loadedTextureNames[j]) {
                    textures.push_back(loadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                if (texPath.find("baseColor") != std::string::npos || texPath.find("diffuse") != std::string::npos) {
                    VEGATexture* texture = new VEGATexture((fileDir + texPath).c_str(), "diffuse", loadedTextures.size());
                    textures.push_back(texture);
                    loadedTextures.push_back(texture);
                    loadedTextureNames.push_back(texPath);
                }

                else if (texPath.find("metallicRoughness") != std::string::npos || texPath.find("specular") != std::string::npos) {
                    VEGATexture* texture = new VEGATexture((fileDir + texPath).c_str(), "specular", loadedTextures.size());
                    textures.push_back(texture);
                    loadedTextures.push_back(texture);
                    loadedTextureNames.push_back(texPath);
                }
            }
        } return textures;
    }

    std::vector<VEGAVertex> VEGAModel::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords) {
        std::vector<VEGAVertex> vertices;

        for (uint32_t i = 0; i < positions.size(); i++) {
            vertices.push_back(VEGAVertex(positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), texCoords[i]));
        } return vertices;
    }

    std::vector<glm::vec2> VEGAModel::groupFloatsVec2(std::vector<float> floats) {
        std::vector<glm::vec2> vectors;

        for (uint32_t i = 0; i < floats.size(); i) {
            vectors.push_back(glm::vec2(floats[i++], floats[i++]));
        } return vectors;
    }

    std::vector<glm::vec3> VEGAModel::groupFloatsVec3(std::vector<float> floats) {
        std::vector<glm::vec3> vectors;

        for (uint32_t i = 0; i < floats.size(); i) {
            vectors.push_back(glm::vec3(floats[i++], floats[i++], floats[i++]));
        } return vectors;
    }

    std::vector<glm::vec4> VEGAModel::groupFloatsVec4(std::vector<float> floats) {
        std::vector<glm::vec4> vectors;

        for (uint32_t i = 0; i < floats.size(); i) {
            vectors.push_back(glm::vec4(floats[i++], floats[i++], floats[i++], floats[i++]));
        } return vectors;
    }

    void VEGAModel::destroy() {
        for (uint32_t i = 0; i < loadedMeshes.size(); i++) {
            loadedMeshes[i]->destroy();
            delete loadedMeshes[i];
        }

        for (uint32_t i = 0; i < loadedTextures.size(); i++) {
            loadedTextures[i]->destroy();
            delete loadedTextures[i];
        }
    }
}