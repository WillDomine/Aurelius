#pragma once
#include "DeviceService.h"
#include "SwapChainService.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class PipelineService {
public:
    PipelineService(DeviceService& deviceService, SwapChainService& swapChainService);
    ~PipelineService();

    PipelineService(const PipelineService&) = delete;
    PipelineService& operator=(const PipelineService&) = delete;

    VkPipeline getPipeline() { return graphicsPipeline; }
    VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
    VkRenderPass getRenderPass() { return renderPass; }
    
    VkFramebuffer getFramebuffer(int index) { return swapChainFramebuffers[index]; }
    VkDescriptorSetLayout getDescriptorSetLayout() { return descriptorSetLayout; }

    void recreateFramebuffers();

private:
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();

    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

    DeviceService& deviceService;
    SwapChainService& swapChainService;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkDescriptorSetLayout descriptorSetLayout;
    void createDescriptorSetLayout();
};