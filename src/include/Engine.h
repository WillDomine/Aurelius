#pragma once
#ifndef AURELIUS_ENGINE_H
#define AURELIUS_ENGINE_H
#include <vulkan/vulkan_core.h>
#include <optional>
#include <vector>
#include "GLFW/glfw3.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

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
    //Check if the required extension supports swap chains
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    //Find the queue families
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    //Swap Chain Support Details
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
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
    VkQueue presentQueue;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};


#endif //PLUTRON_ENGINE_H