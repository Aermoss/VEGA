#include <vega/texture.hpp>

namespace vega {
    VEGATexture::VEGATexture(const char* file, const char* type, unsigned int unit) : unit(unit), type(type) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* bytes = stbi_load(file, &width, &height, &channels, 0);

        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

        else if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

        else if (channels == 1)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(bytes);
    }

    VEGATexture::~VEGATexture() {

    }

    void VEGATexture::texUnit(unsigned int id, unsigned int unit) {
        glUniform1i(id, unit);
    }

    void VEGATexture::bind() {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void VEGATexture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void VEGATexture::destroy() {
        glDeleteTextures(1, &id);
    }

    const char* VEGATexture::getType() {
        return type;
    }
}