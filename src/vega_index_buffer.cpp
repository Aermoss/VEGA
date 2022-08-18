#include "vega_index_buffer.hpp"

namespace vega {
    VEGAIndexBuffer::VEGAIndexBuffer(std::vector <unsigned int>& indices) : count(indices.size()) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    VEGAIndexBuffer::~VEGAIndexBuffer() {
        destroy();
    }

    int VEGAIndexBuffer::get_count() {
        return count;
    }

    void VEGAIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void VEGAIndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void VEGAIndexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }
}