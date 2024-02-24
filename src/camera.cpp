#include <vega/camera.hpp>

namespace vega {
    VEGACamera::VEGACamera(VEGAWindow* window, float fov, float near, float far, float sensitivity, float normalSpeed, float sprintSpeed, glm::vec3 position)
        : window(window), fov(fov), near(near), far(far), sensitivity(sensitivity), normalSpeed(normalSpeed), sprintSpeed(sprintSpeed), position(position) {}

    VEGACamera::~VEGACamera() {

    }

    void VEGACamera::updateMatrices(VEGAShader* shader) {
        int width, height;
        window->getSize(&width, &height);

        glm::mat4 view = glm::lookAt(position, position + front, up);
        glm::mat4 proj = glm::perspective(glm::radians(fov), (float) (width / height), near, far);

        glUniformMatrix4fv(shader->getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(shader->getUniformLocation("cameraPosition"), 1, (float*) glm::value_ptr(getPosition()));
    }

    void VEGACamera::processInputs() {
        speed = window->input->getKey(GLFW_KEY_LEFT_CONTROL) ? sprintSpeed : normalSpeed;
        glm::vec3 direction = glm::vec3(glm::cos(glm::radians(yaw)), 0.0f, glm::sin(glm::radians(yaw)));

        if (window->input->getKey(GLFW_KEY_W))
            position += speed * direction * window->deltaTime;

        if (window->input->getKey(GLFW_KEY_S))
            position += speed * -direction * window->deltaTime;

        if (window->input->getKey(GLFW_KEY_D))
            position += speed * glm::normalize(glm::cross(direction, up)) * window->deltaTime;

        if (window->input->getKey(GLFW_KEY_A))
            position += speed * -glm::normalize(glm::cross(direction, up)) * window->deltaTime;

        if (window->input->getKey(GLFW_KEY_SPACE))
            position += speed * up * window->deltaTime;

        if (window->input->getKey(GLFW_KEY_LEFT_SHIFT))
            position += speed * -up * window->deltaTime;

        double x, y;
        window->input->getMousePosition(&x, &y);
        window->input->setMouseVisible(false);

        int32_t width, height;
        window->getSize(&width, &height);

        float xOffset = sensitivity * (float) (x - width / 2) / width;
        float yOffset = sensitivity * (float) (y - height / 2) / height;

        yaw += xOffset;
        pitch = glm::clamp(pitch - yOffset, -89.9f, 89.9f);

        front = glm::normalize(glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
        )); window->input->setMousePosition(width / 2, height / 2);
    }

    glm::vec3& VEGACamera::getPosition() {
        return position;
    }
}