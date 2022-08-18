#include "vega_shader.hpp"

namespace vega {
    VEGAShader::VEGAShader(std::string vertex_source_string, std::string fragment_source_string) {
        id = glCreateProgram();

        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_source = vertex_source_string.c_str();
        const char* fragment_source = fragment_source_string.c_str();

        glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
        glShaderSource(fragment_shader, 1, &fragment_source, nullptr);

        glCompileShader(vertex_shader);
        check_shader(vertex_shader, "vertex");
        glCompileShader(fragment_shader);
        check_shader(fragment_shader, "fragment");

        glAttachShader(id, vertex_shader);
        glAttachShader(id, fragment_shader);
        glLinkProgram(id);
        check_program();

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    VEGAShader::~VEGAShader() {
        destroy();
    }

    void VEGAShader::check_program() {
        int status;
        char log[1024];

        glGetProgramiv(id, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            glGetProgramInfoLog(id, 1024, nullptr, log);
            std::cout << log << std::endl;
        }
    }

    void VEGAShader::check_shader(unsigned int shader, const char* type) {
        int status;
        char log[1024];
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, nullptr, log);
            std::cout << log << std::endl;
        }
    }

    int VEGAShader::get_uniform_location(const char* name) {
        return glGetUniformLocation(id, name);
    }

    void VEGAShader::use() {
        glUseProgram(id);
    }

    void VEGAShader::destroy() {
        glDeleteProgram(id);
    }

}