#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <string>

class WindowService {
    public:
        WindowService(int width, int height, std::string name);
        ~WindowService();

        WindowService(const WindowService&) = delete;
        WindowService& operator=(const WindowService&) = delete;

        bool shouldClose() {return glfwWindowShouldClose(window); };

        bool wasWindowResized() { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }

        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        GLFWwindow* getGLFWwindow() const { return window; };

private:
    private:
        void initWindow();

        bool framebufferResized = false;
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        int width;
        int height;
        std::string windowName;
        GLFWwindow* window;
};