//
// Created by Griffin Roberts on 4/12/26.
//

#include "VulkanBackend.h"

#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace VulkanBackend {

    // initializing vulkan
    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "EcocystemSim";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_4;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

    //     std::vector<const char*> requiredExtensions;
    //
    //     for (uint32_t i=0; i < glfwExtensionCount; i++) {
    //         requiredExtensions.emplace_back(glfwExtensions[i]);
    //     }
    //
    //     requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    //
    //     createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    //
    //     createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    //     createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    //
    //     if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to create instance!");
    //     }
    }

    void initVulkan() {
        createInstance();
    }

    // initializing GLFW
    void initGLFW() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Ecosystem Sim", nullptr, nullptr);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    void run() {
        initGLFW();
        initVulkan();
        mainLoop();
        cleanup();
    }

    int main() {
        try {
            run();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
} // VulkanBackend