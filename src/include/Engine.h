#pragma once
#ifndef AURELIUS_ENGINE_H
#define AURELIUS_ENGINE_H
#include <vulkan/vulkan_core.h>
#include <optional>
#include <vector>
#include "GLFW/glfw3.h"

struct QueueFamilyIndices {
    //Graphics (calcs the pixels)
    std::optional<uint32_t> graphicsFamily;
    //Present (moves rendered frame to monitor)
    std::optional<uint32_t> presentFamily;
    //Computes complex (calcs will be used for physics)
    std::optional<uint32_t> computeFamily; 
    //Transfer (Used for moving data from staging buffer)
    std::optional<uint32_t> transferFamily; 

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value() && 
               computeFamily.has_value() && transferFamily.has_value();
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
    //Get Vulkan API Version
    uint32_t getSupportedInstanceVersion();
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
    //Get surface aka color view
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &avaliableFormats);
    //Get the present mode ideally Mailbox
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &avaliablePresentModes);
    //Set the screen size for swap chain have to destory if this changes.
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabiltes);
    //create Image Views
    void createImageViews();
    //Create swapchain
    void createSwapChain();
    //Main game engine loop
    void mainLoop();
    //Clean up the game engine
    void cleanup();

    //Core Window
    GLFWwindow* window;
    VkSurfaceKHR surface;
    VkInstance instance;
    //Hardware
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    //Queues the parallel lanes
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;
    //Swap Chain
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    //Window size (for testing) full-screen integration later
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};


#endif