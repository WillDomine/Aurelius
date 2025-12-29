#pragma once
#include <vulkan/vulkan_core.h>
#include "vk_mem_alloc.h"
#include <vector>

struct Mesh {
    VkBuffer vertexBuffer;
    VmaAllocation vertexAllocation;

    VkBuffer indexBuffer;
    VmaAllocation indexAllocation;

    uint32_t indexCount;
};