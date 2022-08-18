#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace vega {
    class VEGATexture {
        private:
            unsigned int id;
            unsigned int unit;
            const char* type;
            
        public:
            VEGATexture(const char* file, const char* type, unsigned int unit);

            void tex_unit(unsigned int id, unsigned int unit);
            void bind();
            void unbind();
            void destroy();
            const char* get_type();
    };
}