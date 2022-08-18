#include "vega_camera.hpp"

namespace vega {
    VEGACamera::VEGACamera(VEGAWindow* window, float fov, float near, float far, float sensitivity, float normal_speed, float sprint_speed, glm::vec3 position)
        : window(window), fov(fov), near(near), far(far), sensitivity(sensitivity), normal_speed(normal_speed), sprint_speed(sprint_speed), position(position) {}

    void VEGACamera::handle_matrix(int id) {
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        int width, height;
        window->get_size(&width, &height);

        view = glm::lookAt(position, position + front, up);
        proj = glm::perspective(glm::radians(fov), (float) (width / height), near, far);

        glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(proj * view));
    }

    void VEGACamera::handle_events() {
        if (window->input->get_key(GLFW_KEY_W)) {
            position += speed * front;
        }

        if (window->input->get_key(GLFW_KEY_S)) {
            position += speed * -front;
        }

        if (window->input->get_key(GLFW_KEY_D)) {
            position += speed * glm::normalize(glm::cross(front, up));
        }

        if (window->input->get_key(GLFW_KEY_A)) {
            position += speed * -glm::normalize(glm::cross(front, up));
        }

        if (window->input->get_key(GLFW_KEY_SPACE)) {
            position += speed * up;
        }

        if (window->input->get_key(GLFW_KEY_LEFT_SHIFT)) {
            position += speed * -up;
        }

        if (window->input->get_key(GLFW_KEY_LEFT_CONTROL)) {
            speed = sprint_speed;
        }

        else {
            speed = normal_speed;
        }

        window->input->set_mouse_visible(false);

        double x, y;
        window->input->get_mouse_pos(&x, &y);

        int width, height;
        window->get_size(&width, &height);

        float x_offset = sensitivity * (float) (x - width / 2) / width;
        float y_offset = sensitivity * (float) (y - height / 2) / height;

        yaw += x_offset;
        pitch -= y_offset;

        if (pitch >= 89.0f)
            pitch = 89.0f;

        if (pitch <= -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        window->input->set_mouse_pos(width / 2, height / 2);
    }

    glm::vec3* VEGACamera::get_pos() {
        return &position;
    }
}