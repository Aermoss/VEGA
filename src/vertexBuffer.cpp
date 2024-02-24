#include <vega/vertexBuffer.hpp>

namespace vega {
    VEGAVertexBuffer::VEGAVertexBuffer(std::vector <VEGAVertex>& vertices) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VEGAVertex), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VEGAVertexBuffer::~VEGAVertexBuffer() {

    }

    void VEGAVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VEGAVertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VEGAVertexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }
}