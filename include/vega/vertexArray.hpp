#pragma once

#include <glad/glad.h>

#include "vertexBuffer.hpp"

namespace vega {
    class VEGAVertexArray {
        private:
            unsigned int id;

        public:
            VEGAVertexArray();
            ~VEGAVertexArray();

            void linkAttrib(VEGAVertexBuffer* buffer, unsigned int index, GLenum type, unsigned int count, unsigned int stride, void* offset);
            void bind();
            void unbind();
            void destroy();
    };
}