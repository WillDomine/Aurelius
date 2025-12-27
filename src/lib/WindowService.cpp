#include "../include/WindowService.h"
#include <stdexcept>

WindowService::WindowService(int w, int h, std::string name) : width(w), height(h), windowName(name) {
    initWindow();
}

WindowService::~WindowService() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowService::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

void WindowService::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}