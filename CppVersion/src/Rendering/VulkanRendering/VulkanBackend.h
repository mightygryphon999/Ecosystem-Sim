//
// Created by Griffin Roberts on 4/12/26.
//

#ifndef C__VERSION_VULKANBACKEND_H
#define C__VERSION_VULKANBACKEND_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace VulkanBackend {
    inline GLFWwindow* window;
    constexpr uint32_t WIDTH = 800;
    constexpr uint32_t HEIGHT = 600;

    inline VkInstance instance;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    inline VkDevice device;
    inline VkQueue graphicsQueue;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    extern VkDebugUtilsMessengerEXT debugMessenger;

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };

    int main();
} // VulkanBackend

#endif //C__VERSION_VULKANBACKEND_H
