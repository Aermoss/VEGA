#pragma once

#include <glad/glad.h>

#include <vector>

namespace vega {
    class VEGAIndexBuffer {
        private:
            unsigned int id;
            unsigned int count;

        public:
            VEGAIndexBuffer(std::vector <unsigned int>& indices);
            ~VEGAIndexBuffer();

            int getCount();
            void bind();
            void unbind();
            void destroy();
    };
}