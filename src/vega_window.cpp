#include "vega_window.hpp"

namespace vega {
    VEGAWindow::VEGAWindow(int width, int height, const char* title, const char* icon_path, VEGAColor background_color, bool fullscreen, bool resizable, bool vsync, bool counter)
        : width(width), height(height), icon_path(icon_path), background_color(background_color), title(title), resizable(resizable), vsync(vsync), counter(counter) {

        glfwInit();
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (width == -1) { // bug with no curly braces
            width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
        }

        if (height == -1) {
            height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
        }

        window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        glfwMakeContextCurrent(window);

        if (icon_path != nullptr) {
            GLFWimage images[1];
            stbi_set_flip_vertically_on_load(false);
            images[0].pixels = stbi_load(icon_path, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(window, 1, images); 
            stbi_image_free(images[0].pixels);
        }

        glfwSwapInterval(vsync ? 1 : 0);
        gladLoadGL();
        glViewport(0, 0, width, height);
        glClearColor(background_color.get_rf(), background_color.get_gf(), background_color.get_bf(), background_color.get_af());

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);

        input = new VEGAInput(window);
    }

    VEGAWindow::~VEGAWindow() {
        destroy();
    }

    bool VEGAWindow::should_close() {
        return glfwWindowShouldClose(window);
    }

    int VEGAWindow::check_errors(bool log) {
        int error = glGetError();

        if (error && log)
            std::cout << "[ERROR]: " << error << std::endl;

        return error;
    }

    float VEGAWindow::get_fps() {
        return fps;
    }

    float VEGAWindow::get_ms() {
        return ms;
    }

    void VEGAWindow::maximize() {
        glfwMaximizeWindow(window);
    }

    void VEGAWindow::iconify() {
        glfwIconifyWindow(window);
    }

    void VEGAWindow::destroy() {
        delete input;

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VEGAWindow::clear_color() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void VEGAWindow::swap_buffers() {
        glfwSwapBuffers(window);
    }

    void VEGAWindow::handle_events() {
        glfwPollEvents();
        current_time = glfwGetTime();
        frames++;

        if (resizable) {
            get_size(&width, &height);
            update_viewport();
        }

        if (current_time - previous_time >= 1.0f / 30.0f) {
            fps = (1.0f / (current_time - previous_time)) * frames;
            ms = ((current_time - previous_time) / frames) * 1000.0f;
            previous_time = current_time;
            frames = 0;

            if (counter) {
                std::string new_title = std::string(title) + " - " + std::to_string(fps) + " " + "fps" + " | " + std::to_string(ms) + " " + "ms";
                glfwSetWindowTitle(window, new_title.c_str());
            }
        }
    }

    const char* VEGAWindow::get_icon_path() {
        return icon_path;
    }

    void VEGAWindow::set_icon(const char* icon_path) {
        if (icon_path != nullptr) {
            GLFWimage images[1];
            stbi_set_flip_vertically_on_load(false);
            images[0].pixels = stbi_load(icon_path, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(window, 1, images); 
            stbi_image_free(images[0].pixels);
        }

        this->icon_path = icon_path;
    }

    const char* VEGAWindow::get_title() {
        return title;
    }

    void VEGAWindow::set_title(const char* title) {
        this->title = title;
    }

    void VEGAWindow::update_viewport() {
        glViewport(0, 0, width, height);
    }

    void VEGAWindow::get_size(int* width, int* height) {
        glfwGetWindowSize(window, width, height);
    }

    void VEGAWindow::set_size(int width, int height) {
        glfwSetWindowSize(window, width, height);
        glViewport(0, 0, width, height);
    }

    VEGAColor* VEGAWindow::get_background_color() {
        return &background_color;
    }

    void VEGAWindow::set_background_color(VEGAColor background_color) {
        this->background_color = background_color;
    }

    void VEGAWindow::set_vsync(bool vsync) {
        glfwSwapInterval(vsync ? 1 : 0);
        this->vsync = vsync;
    }

    void VEGAWindow::set_counter(bool counter) {
        this->counter = counter;
    }

    bool VEGAWindow::get_vsync() {
        return vsync;
    }

    bool VEGAWindow::get_counter() {
        return counter;
    }

    bool VEGAWindow::get_resizable() {
        return resizable;
    }

    GLFWwindow* VEGAWindow::get() {
        return window;
    }
}