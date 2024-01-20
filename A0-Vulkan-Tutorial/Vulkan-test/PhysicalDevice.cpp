#include "WindowShow.h"

// ----------------------------- Physical Devices --------------------------------
bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device) {
    //VkPhysicalDeviceProperties deviceProperties;
    //VkPhysicalDeviceFeatures deviceFeatures;
    //vkGetPhysicalDeviceProperties(device, &deviceProperties);
    //vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    //// here we want device to support "geometry shader"
    //return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
    //    deviceFeatures.geometryShader;

    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extenstionSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extenstionSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extenstionSupported && swapChainAdequate;

    // return true;
}

bool HelloTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

int HelloTriangleApplication::rateDeviceSuitability(VkPhysicalDevice device) {
    int score = 0;

    // Discrete GPUs have a significant performance advantage
    /*if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }*/

    // Maximum possible size of textures affects graphics quality
    //score += deviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    /*if (!deviceFeatures.geometryShader) {
        return 0;
    }*/

    return score;
}

void HelloTriangleApplication::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    //// or we can pick a higher score device
    //std::multimap<int, VkPhysicalDevice> candidates;
    //for (const auto& device : devices) {
    //    int score = rateDeviceSuitability(device);
    //    candidates.insert(std::make_pair(score, device));
    //}
    //// Check if the best candidate is suitable at all
    //if (candidates.rbegin()->first > 0) {
    //    physicalDevice = candidates.rbegin()->second;
    //}
    //else {
    //    throw std::runtime_error("failed to find a suitable GPU!");
    //}

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}
