# VEGAEngine
A game engine made with C++ using OpenGL

# Examples
## Creating a window
```c++
#include <iostream>
#include <vector>
#include <string>

#include "vega.hpp"

static float ambient = 0.0f;
static vega::VEGAShader* shader;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ambient += yoffset / 10.0f;

    if (ambient >= 0.99f)
        ambient = 0.99f;

    if (ambient <= 0.01f)
        ambient = 0.01f;

    std::cout << "ambient: " << ambient << std::endl;

    shader->use();
    glUniform1f(shader->get_uniform_location("ambient"), ambient);
}

int main(int argc, const char* argv[]) {
    vega::VEGAWindow window(1200, 600, "VEGA Window", "res/icon/icon.png", vega::VEGAColor(0.0f, 0.0f, 0.0f), false, false, true, true);

    shader = new vega::VEGAShader(
        vega::VEGAReadFile("shaders/default.vert"),
        vega::VEGAReadFile("shaders/default.frag")
    );

    vega::VEGAModel crow("res/models/crow/scene.gltf");
    vega::VEGACamera camera(&window, 45.0f, 0.01f, 1000.0f, 200.0f, 0.05f, 0.1f, glm::vec3(0.0f, 0.0f, -2.0f));

    // model.transform.scale(glm::vec3(0.5f, 0.8f, 0.5f));

    glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 light_pos = glm::vec3(0.0f, 0.5f, 0.0f);

    shader->use();
    glUniform4f(shader->get_uniform_location("light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
    glUniform3f(shader->get_uniform_location("light_position"), light_pos.x, light_pos.y, light_pos.z);
    glUniform1f(shader->get_uniform_location("ambient"), ambient);

    glfwSetScrollCallback(window.get(), scroll_callback);

    while (!window.should_close()) {
        window.handle_events();
        camera.handle_events();
        window.clear_color();

        if (window.input->get_mouse_button(GLFW_MOUSE_BUTTON_LEFT)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (window.input->get_mouse_button(GLFW_MOUSE_BUTTON_MIDDLE)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

        if (window.input->get_mouse_button(GLFW_MOUSE_BUTTON_RIGHT)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        /* if (window.input->get_key(GLFW_KEY_Q)) {
            model.transform.rotate(-1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (window.input->get_key(GLFW_KEY_E)) {
            model.transform.rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        } */

        crow.render(*shader, camera);

        window.check_errors();
        window.swap_buffers();
    }

    delete shader;
    return 0;
}
```
