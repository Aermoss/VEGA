# VEGA Engine
A simple 3D game engine written in C++ using OpenGL.

# Examples
## Creating a window.
```c++
#include <vega/vega.hpp>

int main(int argc, const char* argv[]) {
    vega::VEGAWindow window(1200, 600, "VEGA Window");

    while (!window.shouldClose()) {
        window.pollEvents();
        window.clear();
        window.checkErrors();
        window.swapBuffers();
    }
    
    window.destroy();
    return 0;
}
```

## Rendering a model.
```c++
#include <vega/vega.hpp>

int main(int argc, const char* argv[]) {
    vega::VEGAWindow window(1200, 600, "VEGA Window");

    vega::VEGAShader shader(
        vega::VEGAReadFile("shaders/default.vert"),
        vega::VEGAReadFile("shaders/default.frag")
    );

    vega::VEGACamera camera(&window, 45.0f, 0.1f, 1000.0f);
    vega::VEGAModel model("res/models/crow/scene.gltf");
    
    shader.use();
    glUniform4fv(shader.getUniformLocation("lightColor"), 1, (float*) glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    glUniform3fv(shader.getUniformLocation("lightPosition"), 1, (float*) glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
    glUniform1f(shader.getUniformLocation("ambient"), ambient);
    shader.unuse();

    while (!window.shouldClose()) {
        window.pollEvents();
        camera.processInputs();
        window.clear();
        model.render(&shader, &camera);
        window.checkErrors();
        window.swapBuffers();
    }

    model.destroy();
    shader.destroy();
    window.destroy();
    return 0;
}
```