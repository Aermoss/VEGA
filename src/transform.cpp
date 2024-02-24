#include <vega/transform.hpp>

namespace vega {
    VEGATransform::VEGATransform() {
        reset();
    }

    VEGATransform::~VEGATransform() {

    }

    void VEGATransform::translate(glm::vec3 value) {
        transform = glm::translate(transform, value);
    }

    void VEGATransform::rotate(float angle, glm::vec3 axis) {
        transform = glm::rotate(transform, glm::radians(angle), axis);
    }

    void VEGATransform::scale(glm::vec3 value) {
        transform = glm::scale(transform, value);
    }

    void VEGATransform::reset() {
        transform = glm::mat4(1.0f);
    }

    void VEGATransform::uniform(unsigned int id) {
        glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(transform));
    }

    glm::mat4* VEGATransform::get() {
        return &transform;
    }
    void VEGATransform::set(glm::mat4* matrix) {
        transform = *matrix;
    }

    void VEGATransform::destroy() {

    }
}