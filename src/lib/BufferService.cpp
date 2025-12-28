#include "../include/BufferService.h"
#include <stdexcept>
#include <cstring>


//Square Data
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0: Top-Left (Red)
    {{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 1: Top-Right (Green)
    {{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // 2: Bottom-Right (Blue)
    {{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}  // 3: Bottom-Left (White)
};

//Square Draw Order
const std::vector<uint16_t> indices = {
    0, 1, 2, // First Triangle
    2, 3, 0  // Second Triangle
};

BufferService::BufferService(DeviceService& device) : deviceService(device) {
    createVertexBuffer();
    createIndexBuffer();
}

BufferService::~BufferService() {
    vkDestroyBuffer(deviceService.device(), indexBuffer, nullptr);
    vkFreeMemory(deviceService.device(), indexBufferMemory, nullptr);
    vkDestroyBuffer(deviceService.device(), vertexBuffer, nullptr);
    vkFreeMemory(deviceService.device(), vertexBufferMemory, nullptr);
}

void BufferService::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(bufferSize, 
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(deviceService.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(deviceService.device(), stagingBufferMemory);

    createBuffer(bufferSize, 
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                 vertexBuffer, vertexBufferMemory);

    deviceService.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(deviceService.device(), stagingBuffer, nullptr);
    vkFreeMemory(deviceService.device(), stagingBufferMemory, nullptr);
}

void BufferService::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // A. Staging Buffer (CPU -> Transfer Queue)
    createBuffer(bufferSize, 
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(deviceService.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(deviceService.device(), stagingBufferMemory);

    // B. Index Buffer (Transfer Queue -> Graphics Queue)
    // Note: Usage is VK_BUFFER_USAGE_INDEX_BUFFER_BIT
    createBuffer(bufferSize, 
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                 indexBuffer, indexBufferMemory);

    // C. Copy (DeviceService handles the queues and barriers automatically!)
    deviceService.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(deviceService.device(), stagingBuffer, nullptr);
    vkFreeMemory(deviceService.device(), stagingBufferMemory, nullptr);
}

void BufferService::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    if (vkCreateBuffer(deviceService.device(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(deviceService.device(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = deviceService.findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(deviceService.device(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(deviceService.device(), buffer, bufferMemory, 0);
}