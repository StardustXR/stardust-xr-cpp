#ifndef STARDUSTVULKAN_H
#define STARDUSTVULKAN_H

#include <QObject>

#include "openxr_meta.h"

namespace Stardust {

class OpenXR;

class Vulkan : public QObject {
    Q_OBJECT
    Q_PROPERTY(OpenXR *openxr MEMBER openxr)
public:
    explicit Vulkan(QObject *parent = nullptr);
    ~Vulkan();

    OpenXR *openxr = nullptr;
    Q_INVOKABLE void initialize();

    uint32_t instanceExtensionCount = 0;
    const char* const* instanceExtensions = nullptr;
    VkInstanceCreateInfo instanceInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr};
    VkInstance instance = VK_NULL_HANDLE;

    VkDebugUtilsMessengerEXT debugMessenger;

    uint32_t physicalDeviceCount = 0;
    std::vector<VkPhysicalDevice> *physicalDevices;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDeviceQueueCreateInfo queueInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr};
    uint32_t queueFamilyIndex = 0;
    uint32_t queueIndex = 0;
    VkQueue *queue = new VkQueue;

    std::vector<const char*> deviceExtensions;

    VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr};
    VkDevice device = VK_NULL_HANDLE;

    VkCommandPool pool = VK_NULL_HANDLE;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

private:
    bool isDeviceSuitable(VkPhysicalDevice device);
    uint32_t findQueueFamily(VkPhysicalDevice device);
    uint GetExtensionNumber(uint32_t length, char *names);
    std::vector<const char*> ParseExtensionString(char* names);
    bool checkValidationLayerSupport();
};

}

#endif // STARDUSTVULKAN_H
