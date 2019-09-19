#include "stardustvulkan.h"

#include <QDebug>
#include "stardustopenxr.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Stardust {

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

Vulkan::Vulkan(QObject *parent) : QObject(parent) {

}

Vulkan::~Vulkan() {
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroyInstance(instance, nullptr);
}

void Vulkan::initialize() {
    //Get the vulkan instance version
    uint32_t version;
    vkEnumerateInstanceVersion(&version);
    qDebug() << "Version: " << ((version >> 22) & 0x3ff) << '.' << ((version >> 12) & 0x3ff) << '.' << (version & 0x0fff);

    //Get Vulkan physical device
    XrGraphicsRequirementsVulkanKHR graphicsRequirements{XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR};
    xrGetVulkanGraphicsRequirementsKHR(*openxr->xrInstance, *openxr->hmdID, &graphicsRequirements);

    //Get vulkan graphics requirements for OpenXR
    graphicsRequirements = {XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR};
    xrGetVulkanGraphicsRequirementsKHR(*openxr->xrInstance, *openxr->hmdID, &graphicsRequirements);

    //Get all vulkan extensions needed for OpenXR
    uint32_t extensionNamesSize = 0;
    xrGetVulkanInstanceExtensionsKHR(*openxr->xrInstance, *openxr->hmdID, 0, &extensionNamesSize, nullptr);
    std::vector<char> extensionNames(extensionNamesSize);
    xrGetVulkanInstanceExtensionsKHR(*openxr->xrInstance, *openxr->hmdID, extensionNamesSize, &extensionNamesSize, extensionNames.data());

    //Covert all extensions into the correct format
    std::vector<const char*> instanceExtensions = ParseExtensionString(extensionNames.data());

    //Get GLFW extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for(uint i=0; i<glfwExtensionCount; i++) {
        instanceExtensions.push_back(glfwExtensions[i]);
    }

    //Add Vulkan debug extension
    if (enableValidationLayers) {
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    //Add Vulkan external memory extensions
    instanceExtensions.push_back(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);

    //Get vulkan instance layers
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    //VkInstanceCreateInfo *instanceInfo
    instanceInfo.pNext = nullptr;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = nullptr;
    if (enableValidationLayers) {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        instanceInfo.enabledLayerCount = 0;
    }
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    instanceInfo.ppEnabledExtensionNames = instanceExtensions.size() ? &instanceExtensions[0] : nullptr;

    VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);

    //Create debug info
    if (enableValidationLayers) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    xrGetVulkanGraphicsDeviceKHR(*openxr->xrInstance, *openxr->hmdID, instance, &physicalDevice);

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
    queueFamilyIndex = findQueueFamily(physicalDevice);
    queueIndex = 0;

    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueInfo.pQueuePriorities = &queuePriority;

    //Get vulkan device extensions
    uint32_t deviceExtensionNamesSize = 0;
    xrGetVulkanDeviceExtensionsKHR(*openxr->xrInstance, *openxr->hmdID, 0, &deviceExtensionNamesSize, nullptr);
    std::vector<char> deviceExtensionNames(deviceExtensionNamesSize);
    xrGetVulkanDeviceExtensionsKHR(*openxr->xrInstance, *openxr->hmdID, deviceExtensionNamesSize, &deviceExtensionNamesSize, &deviceExtensionNames[0]);
    deviceExtensions = ParseExtensionString(&deviceExtensionNames[0]);

    deviceExtensions.push_back(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME);
    deviceExtensions.push_back(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME);

    //Create vulkan graphics device
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.enabledLayerCount = 0;

    result = vkCreateDevice(physicalDevice, &deviceCreateInfo, VK_NULL_HANDLE, &device);

    vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, queue);

    VkCommandPoolCreateInfo cmdPoolInfo = {
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        nullptr,
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        queueFamilyIndex
    };

    vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &pool);

    qDebug() << "finished vulkan initialization" << endl;
}

uint32_t Vulkan::findQueueFamily(VkPhysicalDevice device) {
    uint32_t i = 0;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            break;

        i++;
    }

    return i;
}



uint Vulkan::GetExtensionNumber(uint32_t length, char *names) {
    uint space = 0;
    for(uint i=0; i<length; i++) {
        if(names[i] == ' ') {
            space++;
        }
    }

    return ++space;
}

//const char* const* Vulkan::ParseExtensionString(uint extensionCount, uint32_t length, char* names) {
//    char** list = new char*[extensionCount];

//    uint j = 0;
//    uint k = 0;
//    char* name = new char[VK_MAX_EXTENSION_NAME_SIZE];
//    for(uint i=0; i<length; i++) {
//        if(names[i] ==  ' ') {
//            name[k] = '\0';
//            list[j] = name;
//            j++;
//            name = new char[VK_MAX_EXTENSION_NAME_SIZE];
//            k = 0;
//            continue;
//        }
//        name[k] = names[i];
//        k++;
//    }
//    name[k] = '\0';
//    list[j] = name;

//    return list;
//}


std::vector<const char*> Vulkan::ParseExtensionString(char* names) {
    std::vector<const char*> list;
    while (*names) {
        list.push_back(names);
        while (*(++names)) {
            if (*names == ' ') {
                *names++ = '\0';
                break;
            }
        }
    }
    return list;
}

bool Vulkan::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(int i=0; i<availableLayers.size(); i++) {
        qDebug() << availableLayers[i].layerName;
    }

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

}
