#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>

struct Vertex {
    glm::vec3 pos; // WAS vec2, NOW vec3
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        // Position Attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        // CHANGE FORMAT TO R32G32B32_SFLOAT (vec3)
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; 
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // Color Attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};