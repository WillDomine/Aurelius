#pragma once
#include "DeviceService.h"
#include "SwapChainService.h"
#include "PipelineService.h"
#include "BufferService.h"
#include <vulkan/vulkan.h>
#include <vector>

class CommandService {
public:
    CommandService(DeviceService& device, SwapChainService& swapChain, PipelineService& pipeline, BufferService& buffer);
    ~CommandService();

    CommandService(const CommandService&) = delete;
    CommandService& operator=(const CommandService&) = delete;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    uint32_t currentFrame = 0;

    VkResult drawFrame(const Mesh& mesh, VkDescriptorSet descriptorSet);

private:
    void createCommandBuffers();
    void createSyncObjects();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, const Mesh& mesh, VkDescriptorSet descriptorSet);

    DeviceService& deviceService;
    SwapChainService& swapChainService;
    PipelineService& pipelineService;
    BufferService& bufferService;

    std::vector<VkCommandBuffer> commandBuffers;
    
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

};