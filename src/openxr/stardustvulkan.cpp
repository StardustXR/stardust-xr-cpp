#include "stardustvulkan.h"

#include <QDebug>
#include "stardustopenxr.h"

StardustVulkan::StardustVulkan(QObject *parent) : QObject(parent) {

}

void StardustVulkan::initialize() {

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
    xrGetVulkanInstanceExtensionsKHR(*openxr->xrInstance, *openxr->hmdID, extensionNamesSize, &extensionNamesSize, &extensionNames[0]);

    std::vector<const char*> extensions = ParseExtensionString(&extensionNames[0]);

    //VkInstanceCreateInfo *instanceInfo
    instanceInfo.pNext = nullptr;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = nullptr;
    instanceInfo.enabledLayerCount = 0;
//    instanceInfo.enabledExtensionCount = 0;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensions.size() ? &extensions[0] : nullptr;

    vkCreateInstance(&instanceInfo, nullptr, &instance);

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

    //Create vulkan graphics device
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.enabledLayerCount = 0;

    vkCreateDevice(physicalDevice, &deviceCreateInfo, VK_NULL_HANDLE, &device);

    qDebug() << "finished vulkan initialization" << endl;
}

uint32_t StardustVulkan::findQueueFamily(VkPhysicalDevice device) {
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



uint StardustVulkan::GetExtensionNumber(uint32_t length, char *names) {
    uint space = 0;
    for(uint i=0; i<length; i++) {
        if(names[i] == ' ') {
            space++;
        }
    }

    return ++space;
}

//const char* const* StardustVulkan::ParseExtensionString(uint extensionCount, uint32_t length, char* names) {
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


std::vector<const char*> StardustVulkan::ParseExtensionString(char* names) {
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
