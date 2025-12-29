#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "WindowService.h"
#include "DeviceService.h"
#include "BufferService.h"
#include "SwapChainService.h"
#include "PipelineService.h"
#include "CommandService.h"

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

class Engine {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

private: 

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBuffersAllocations;
    std::vector<void*> uniformBuffersMapped;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    void createDescriptorPool();
    void createDescriptorSets();

    //Recreate swap chain on window resize
    void recreateSwapChain();
    //Testing mesh
    Mesh squareMesh;
    // Create the Window
    WindowService windowService{WIDTH, HEIGHT, "AURELIUS ENGINE"};
    // Initialize Vulkan Device (needs Window)
    DeviceService deviceService{windowService};
    // Create the BufferService(needs Device)
    BufferService bufferService{deviceService};
    // Create SwapChain (needs Device + Window)
    SwapChainService swapChainService{deviceService, windowService};
    // Create Pipeline (needs Device + SwapChain)
    PipelineService pipelineService{deviceService, swapChainService};
    // Setup Commands & Drawing (needs Everything)
    CommandService commandService{deviceService, swapChainService, pipelineService, bufferService};
};