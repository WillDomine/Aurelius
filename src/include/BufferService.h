#pragma once
#include "DeviceService.h"
#include "Mesh.h"
#include "Vertex.h"
class BufferService {
public:
    BufferService(DeviceService& deviceService);
    ~BufferService();

    BufferService(const BufferService&) = delete;
    BufferService& operator=(const BufferService&) = delete;

    void createVertexBuffer();
    void createIndexBuffer();

    Mesh uploadMesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

    void destroyMesh(const Mesh& mesh);
    
    VkBuffer getVertexBuffer() { return vertexBuffer; }
    VkBuffer getIndexBuffer() { return indexBuffer; }

private:
    DeviceService& deviceService;

    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferAllocation;

    VkBuffer indexBuffer;
    VmaAllocation indexBufferAllocation;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VkBuffer& buffer, VmaAllocation& allocation);    

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};