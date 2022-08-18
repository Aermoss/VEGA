#pragma once

#include <glad/glad.h>

#include <string>
#include <iostream>

namespace vega {
    class VEGAShader {
        private:
            unsigned int id;

            void check_program();
            void check_shader(unsigned int shader, const char* type);

        public:
            VEGAShader(std::string vertex_source_string, std::string fragment_source_string);
            ~VEGAShader();

            int get_uniform_location(const char* name);
            void use();
            void destroy();
    };
}