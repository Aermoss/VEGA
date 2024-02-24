#include <vega/window.hpp>

namespace vega {
    VEGAWindow::VEGAWindow(int width, int height, const char* title, const char* iconPath, VEGAColor backgroundColor, bool fullscreen, bool resizable, bool vsync)
        : width(width), height(height), iconPath(iconPath), backgroundColor(backgroundColor), title(title), resizable(resizable), vsync(vsync), deltaTime(0.0), currentTime(0.0), lastTime(0.0) {

        glfwInit();
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (width == -1) { // bug with no curly braces
            width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
        }

        if (height == -1) {
            height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
        }

        window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        glfwMakeContextCurrent(window);

        if (iconPath != nullptr) {
            GLFWimage images[1];
            stbi_set_flip_vertically_on_load(false);
            images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(window, 1, images); 
            stbi_image_free(images[0].pixels);
        }

        glfwSwapInterval(vsync ? 1 : 0);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glViewport(0, 0, width, height);
        glClearColor(backgroundColor.get_rf(), backgroundColor.get_gf(), backgroundColor.get_bf(), backgroundColor.get_af());

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);

        input = new VEGAInput(window);
    }

    VEGAWindow::~VEGAWindow() {
        
    }

    bool VEGAWindow::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    int VEGAWindow::checkErrors(bool log) {
        int error = glGetError();

        if (error && log)
            std::cout << "[ERROR]: " << error << std::endl;

        return error;
    }

    void VEGAWindow::destroy() {
        delete input;

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VEGAWindow::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void VEGAWindow::swapBuffers() {
        glfwSwapBuffers(window);
    }

    void VEGAWindow::pollEvents() {
        glfwMakeContextCurrent(window);
        glfwPollEvents();

        int32_t width, height;
        glfwGetWindowSize(window, &width, &height);

        if (width != this->width || height != this->height) {
            this->width = width, this->height = height;
            glViewport(0, 0, width, height);
        }

        currentTime = glfwGetTime();

        if (lastTime == 0.0f)
            lastTime = currentTime;
            
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }

    void VEGAWindow::setIcon(const char* iconPath) {
        if (iconPath != nullptr) {
            GLFWimage images[1];
            stbi_set_flip_vertically_on_load(false);
            images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4);
            glfwSetWindowIcon(window, 1, images); 
            stbi_image_free(images[0].pixels);
        } this->iconPath = iconPath;
    }

    const std::string& VEGAWindow::getIconPath() {
        return iconPath;
    }

    const std::string& VEGAWindow::getTitle() {
        return title;
    }

    void VEGAWindow::setTitle(const char* title) {
        this->title = title;
    }

    void VEGAWindow::getSize(int32_t* width, int32_t* height) {
        glfwGetWindowSize(window, width, height);
    }

    void VEGAWindow::setSize(int32_t width, int32_t height) {
        glfwSetWindowSize(window, width, height);
        glViewport(0, 0, width, height);
    }

    VEGAColor* VEGAWindow::getBackgroundColor() {
        return &backgroundColor;
    }

    void VEGAWindow::setBackgroundColor(VEGAColor backgroundColor) {
        this->backgroundColor = backgroundColor;
    }

    void VEGAWindow::setVsync(bool vsync) {
        glfwSwapInterval(vsync ? 1 : 0);
        this->vsync = vsync;
    }

    bool VEGAWindow::getVsync() {
        return vsync;
    }
    
    GLFWwindow* VEGAWindow::get() {
        return window;
    }
}