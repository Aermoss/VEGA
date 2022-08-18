#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "vega_window.hpp"

namespace vega {
    class VEGACamera {
        private:
            VEGAWindow* window;
            float fov, near, far, sensitivity;
            float speed, normal_speed, sprint_speed;
            float yaw, pitch;

            glm::vec3 position;
            glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        public:
            VEGACamera(VEGAWindow* window, float fov, float near, float far, float sensitivity, float normal_speed, float sprint_speed, glm::vec3 position);
            
            void handle_matrix(int id);
            void handle_events();
            glm::vec3* get_pos();
    };
}