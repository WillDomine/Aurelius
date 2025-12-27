#pragma once
#include "WindowService.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;
    std::optional<uint32_t> transferFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value() && 
               computeFamily.has_value() && transferFamily.has_value();
    }
};

class DeviceService {
    public:
        DeviceService(WindowService& windowService);
        ~DeviceService();

        DeviceService(const DeviceService&) = delete;
        DeviceService& operator=(const DeviceService&) = delete;

        VkDevice device() { return device_; }
        VkSurfaceKHR surface() { return surface_; }
        
        VkQueue graphicsQueue() { return graphicsQueue_; }
        VkQueue presentQueue() { return presentQueue_; }
        VkQueue computeQueue() { return computeQueue_; }
        VkQueue transferQueue() { return transferQueue_; }
        
        VkPhysicalDevice physicalDevice() { return physicalDevice_; }
        VkCommandPool getCommandPool() { return commandPool; }

        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice_); }
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice_); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    private:
        void createInstance();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        VkInstance instance;
        VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
        WindowService& windowService;
        VkCommandPool commandPool;

        VkDevice device_;
        VkSurfaceKHR surface_;
        
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;
        VkQueue computeQueue_;
        VkQueue transferQueue_;

        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};