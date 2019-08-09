#ifndef STARDUSTVULKAN_H
#define STARDUSTVULKAN_H

#include <QObject>
#include <vulkan/vulkan.h>

#define XR_USE_GRAPHICS_API_VULKAN
#define XR_KHR_vulkan_enable
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_platform_defines.h>
#include <openxr/openxr_reflection.h>

class StardustOpenXR;

class StardustVulkan : public QObject {
    Q_OBJECT
    Q_PROPERTY(StardustOpenXR *openxr MEMBER openxr)
public:
    explicit StardustVulkan(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

    uint32_t *instanceExtensionCount = nullptr;
    const char* const* instanceExtensions = nullptr;
    VkInstanceCreateInfo *instanceInfo = new VkInstanceCreateInfo;
    VkInstance *instance = new VkInstance;

    uint32_t *physicalDeviceCount = new uint32_t;
    std::vector<VkPhysicalDevice> *physicalDevices;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures *physicalDeviceFeatures = new VkPhysicalDeviceFeatures;

    VkDeviceQueueCreateInfo *queueInfo = new VkDeviceQueueCreateInfo;
    uint32_t queueFamilyIndex;
    uint32_t queueIndex;

    std::vector<const char*> deviceExtensions;

    VkDeviceCreateInfo *deviceCreateInfo = new VkDeviceCreateInfo;
    VkDevice *device = new VkDevice;

    StardustOpenXR *openxr = nullptr;

private:
    bool isDeviceSuitable(VkPhysicalDevice device);
    uint32_t findQueueFamily(VkPhysicalDevice device);
    uint GetExtensionNumber(uint32_t length, char *names);
    std::vector<const char*> ParseExtensionString(char* names);
};

#endif // STARDUSTVULKAN_H
