//
// Created by Griffin Roberts on 4/11/26.
//

#include "SimVulkanHandler.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace SimVulkanHandler {
    void _init() {
        constexpr vk::ApplicationInfo appInfo{.pApplicationName   = "EcosystemSim",
                                      .applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
                                      .pEngineName        = "EcosystemSim",
                                      .engineVersion      = VK_MAKE_VERSION( 1, 0, 0 ),
                                      .apiVersion         = vk::ApiVersion14};

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo
        };

        instance = vk::raii::Instance(context, createInfo);
    }
    void _main() {

    }
    void _cleanup() {

    }
}