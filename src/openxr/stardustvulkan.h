#ifndef STARDUSTVULKAN_H
#define STARDUSTVULKAN_H

#include <QObject>

#include "openxr_meta.h"

class StardustOpenXR;

class StardustVulkan : public QObject {
    Q_OBJECT
    Q_PROPERTY(StardustOpenXR *openxr MEMBER openxr)
public:
    explicit StardustVulkan(QObject *parent = nullptr);

    StardustOpenXR *openxr = nullptr;
    Q_INVOKABLE void initialize();

    uint32_t instanceExtensionCount = 0;
    const char* const* instanceExtensions = nullptr;
    VkInstanceCreateInfo instanceInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    VkInstance instance = VK_NULL_HANDLE;

    uint32_t physicalDeviceCount = 0;
    std::vector<VkPhysicalDevice> *physicalDevices;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDeviceQueueCreateInfo queueInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    uint32_t queueFamilyIndex = 0;
    uint32_t queueIndex = 0;

    std::vector<const char*> deviceExtensions;

    VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    VkDevice device = VK_NULL_HANDLE;

private:
    bool isDeviceSuitable(VkPhysicalDevice device);
    uint32_t findQueueFamily(VkPhysicalDevice device);
    uint GetExtensionNumber(uint32_t length, char *names);
    std::vector<const char*> ParseExtensionString(char* names);
};

#endif // STARDUSTVULKAN_H
