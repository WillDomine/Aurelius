#define GLFW_INCLUDE_VULKAN
#include <exception>
#include <iostream>
#include <ostream>

#include "include/Engine.h"

int main() {
    try {
        Engine app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}