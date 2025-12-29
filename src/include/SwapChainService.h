#pragma once
#include "DeviceService.h"
#include "WindowService.h"
#include <vulkan/vulkan.h>
#include <vector>

class SwapChainService {
public:
    SwapChainService(DeviceService& deviceService, WindowService& windowService);
    ~SwapChainService();

    SwapChainService(const SwapChainService&) = delete;
    SwapChainService& operator=(const SwapChainService&) = delete;

    VkSwapchainKHR getSwapChain() { return swapChain; }
    VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
    VkExtent2D getSwapChainExtent() { return swapChainExtent; }
    size_t getImageCount() { return swapChainImages.size(); }
    VkImageView getImageView(int index) { return swapChainImageViews[index]; }

    // Check if the swap chain is compatible with the window
    VkResult acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);

    void recreateSwapChain();
    void cleanupSwapChain();

    void createDepthResources();
    VkFormat findDepthFormat();

    VkImageView getDepthImageView() { return depthImageView; }

private:
    void createSwapChain();
    void createImageViews();

    VkImage depthImage;
    VmaAllocation depthImageAllocation;
    VkImageView depthImageView;

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memoryUsage, VkImage& image, VmaAllocation& allocation);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    DeviceService& deviceService;
    WindowService& windowService;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
};