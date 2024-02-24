#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace vega {
    class VEGATransform {
        private:
            glm::mat4 transform;

        public:
            VEGATransform();
            ~VEGATransform();

            void translate(glm::vec3 value);
            void rotate(float angle, glm::vec3 axis);
            void scale(glm::vec3 value);
            void reset();
            void uniform(unsigned int id);
            glm::mat4* get();
            void set(glm::mat4* matrix);
            void destroy();
    };
}