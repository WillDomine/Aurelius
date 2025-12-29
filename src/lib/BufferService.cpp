#include "../include/BufferService.h"
#include <stdexcept>
#include <cstring>


BufferService::BufferService(DeviceService& device) : deviceService(device) {
}

BufferService::~BufferService() {

}

Mesh BufferService::uploadMesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) {
    Mesh mesh{};
    mesh.indexCount = static_cast<uint32_t>(indices.size());

    // --- Vertex Buffer ---
    VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();
    
    VkBuffer stagingBuffer;
    VmaAllocation stagingAlloc;
    
    // 1. Staging
    createBuffer(vertexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer, stagingAlloc);

    void* data;
    vmaMapMemory(deviceService.getAllocator(), stagingAlloc, &data);
    memcpy(data, vertices.data(), (size_t)vertexSize);
    vmaUnmapMemory(deviceService.getAllocator(), stagingAlloc);

    // 2. GPU Buffer
    createBuffer(vertexSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY, mesh.vertexBuffer, mesh.vertexAllocation);

    // 3. Copy
    deviceService.copyBuffer(stagingBuffer, mesh.vertexBuffer, vertexSize);
    vmaDestroyBuffer(deviceService.getAllocator(), stagingBuffer, stagingAlloc);

    // --- Index Buffer ---
    VkDeviceSize indexSize = sizeof(indices[0]) * indices.size();

    createBuffer(indexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer, stagingAlloc);

    vmaMapMemory(deviceService.getAllocator(), stagingAlloc, &data);
    memcpy(data, indices.data(), (size_t)indexSize);
    vmaUnmapMemory(deviceService.getAllocator(), stagingAlloc);

    createBuffer(indexSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY, mesh.indexBuffer, mesh.indexAllocation);

    deviceService.copyBuffer(stagingBuffer, mesh.indexBuffer, indexSize);
    vmaDestroyBuffer(deviceService.getAllocator(), stagingBuffer, stagingAlloc);

    return mesh;
}

void BufferService::destroyMesh(const Mesh& mesh) {
    vmaDestroyBuffer(deviceService.getAllocator(), mesh.indexBuffer, mesh.indexAllocation);
    vmaDestroyBuffer(deviceService.getAllocator(), mesh.vertexBuffer, mesh.vertexAllocation);
}

void BufferService::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VkBuffer& buffer, VmaAllocation& allocation) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = memoryUsage;

    if (vmaCreateBuffer(deviceService.getAllocator(), &bufferInfo, &allocInfo, &buffer, &allocation, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer using VMA!");
    }
}