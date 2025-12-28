#pragma once
#include "DeviceService.h"
#include "SwapChainService.h"
#include "PipelineService.h"
#include <vulkan/vulkan.h>
#include <vector>

class CommandService {
public:
    CommandService(DeviceService& device, SwapChainService& swapChain, PipelineService& pipeline);
    ~CommandService();

    CommandService(const CommandService&) = delete;
    CommandService& operator=(const CommandService&) = delete;

    void drawFrame();

private:
    void createCommandBuffers();
    void createSyncObjects();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    DeviceService& deviceService;
    SwapChainService& swapChainService;
    PipelineService& pipelineService;

    std::vector<VkCommandBuffer> commandBuffers;
    
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;
};