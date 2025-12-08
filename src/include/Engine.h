#ifndef AURELIUS_ENGINE_H
#define AURELIUS_ENGINE_H
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"


class Engine {
public:
    void run();
private:
    //Window Creation
    void initWindow();
    //Vulkan Layer Creation
    void initVulkan();
    //Pick Graphical Device (aka GPU)
    void pickPhysicalDevice();
    //Pick the driver for translation
    void createLogicalDevice();
    //Check if this device is suitable
    bool isDeviceSuitable(VkPhysicalDevice device);
    //Main game engine loop
    void mainLoop();
    //Clean up the game engine
    void cleanup();


    GLFWwindow* window;
    VkSurfaceKHR surface;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue qraphicsQueue;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};


#endif //PLUTRON_ENGINE_H