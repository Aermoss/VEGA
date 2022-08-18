# VEGAEngine
A game engine made with C++ using OpenGL

# Examples
## Creating a window
```c++
#include "vega.hpp"

int main(int argc, const char* argv[]) {
    vega::VEGAWindow window(1200, 600, "VEGA Window", "res/icon/icon.png", vega::VEGAColor(0.0f, 0.0f, 0.0f), false, false, true, true);

    while (!window.should_close()) {
        window.handle_events();
        window.clear_color();
        window.check_errors();
        window.swap_buffers();
    }

    return 0;
}
```
