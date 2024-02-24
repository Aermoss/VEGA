#pragma once

#include <glad/glad.h>

#include <string>
#include <iostream>

namespace vega {
    class VEGAShader {
        private:
            unsigned int id;

            void checkProgram();
            void checkShader(unsigned int shader, const char* type);

        public:
            VEGAShader(std::string vertex_source_string, std::string fragment_source_string);
            ~VEGAShader();

            int getUniformLocation(const char* name);
            void use();
            void unuse();
            void destroy();
    };
}