//
// Created by Griffin Roberts on 4/11/26.
//

#ifndef C__VERSION_SIMVULKANHANDLER_H
#define C__VERSION_SIMVULKANHANDLER_H

#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

namespace SimVulkanHandler {
    inline VkInstance instance{ nullptr };
} // SimVulkanHandler

#endif //C__VERSION_SIMVULKANHANDLER_H
