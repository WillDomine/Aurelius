#include "../include/Engine.h"
#include <iostream>
#include <iomanip> 
#include <chrono>

void Engine::run() {

    std::vector<Vertex> vertices = {
        // Front face (Z = 0.5)
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}}, // 0: Red
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}, // 1: Green
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // 2: Blue
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 3: White
        // Back face (Z = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 4: Red
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 5: Green
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 6: Blue
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}  // 7: White
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0,       // Front
        5, 4, 7, 7, 6, 5,       // Back
        4, 0, 3, 3, 7, 4,       // Left
        1, 5, 6, 6, 2, 1,       // Right
        3, 2, 6, 6, 7, 3,       // Top
        4, 5, 1, 1, 0, 4        // Bottom
    };

    squareMesh = bufferService.uploadMesh(vertices, indices);

    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();

    std::cout << "---------------------------------" << std::endl;
    std::cout << "   AURELIUS ENGINE INITIALIZED   " << std::endl;
    std::cout << "---------------------------------" << std::endl;

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    //Main Loop
    while (!windowService.shouldClose()) {
        //Get Window Events
        glfwPollEvents();

        updateUniformBuffer(commandService.currentFrame);

        //Draw the Frame using the Command Service
        VkResult result = commandService.drawFrame(squareMesh, descriptorSets[commandService.currentFrame]);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowService.wasWindowResized()) {
            windowService.resetWindowResizedFlag();
            recreateSwapChain();
        }

        // 3. FPS Counter Logic
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            std::cout << "\rFPS: " << nbFrames 
                      << " | Frame Time: " << std::fixed << std::setprecision(3) << 1000.0 / double(nbFrames) << "ms" 
                      << "    " << std::flush; // \r allows overwriting the line
            nbFrames = 0;
            lastTime += 1.0;
        }
    }

    // Wait for the GPU to finish the last frame before we kill the services
    vkDeviceWaitIdle(deviceService.device());

    vkDestroyDescriptorPool(deviceService.device(), descriptorPool, nullptr);

    for (size_t i = 0; i < commandService.MAX_FRAMES_IN_FLIGHT; i++) {
        vmaUnmapMemory(deviceService.getAllocator(), uniformBuffersAllocations[i]);
        vmaDestroyBuffer(deviceService.getAllocator(), uniformBuffers[i], uniformBuffersAllocations[i]);
    }

    bufferService.destroyMesh(squareMesh);

    std::cout << "\n\nSHUTTING DOWN..." << std::endl;
}

void Engine::recreateSwapChain() {
    swapChainService.recreateSwapChain();

    pipelineService.recreateFramebuffers();
}

void Engine::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(commandService.MAX_FRAMES_IN_FLIGHT);
    uniformBuffersAllocations.resize(commandService.MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(commandService.MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < commandService.MAX_FRAMES_IN_FLIGHT; i++) {
        bufferService.createBuffer(
            bufferSize, 
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VMA_MEMORY_USAGE_CPU_TO_GPU, //CPU -> GPU
            uniformBuffers[i], 
            uniformBuffersAllocations[i]
        );

        vmaMapMemory(deviceService.getAllocator(), uniformBuffersAllocations[i], &uniformBuffersMapped[i]);
    }
}

void Engine::updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainService.getSwapChainExtent().width / (float) swapChainService.getSwapChainExtent().height, 0.1f, 10.0f);
    
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Engine::createDescriptorPool() {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(commandService.MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(commandService.MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(deviceService.device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

void Engine::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(commandService.MAX_FRAMES_IN_FLIGHT, pipelineService.getDescriptorSetLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(commandService.MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(commandService.MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(deviceService.device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    // Connect the Buffers to the Sets
    for (size_t i = 0; i < commandService.MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0; // Binding 0 in Shader
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(deviceService.device(), 1, &descriptorWrite, 0, nullptr);
    }
}