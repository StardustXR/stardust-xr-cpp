#ifndef STARDUSTOPENXR_H
#define STARDUSTOPENXR_H

#include <QObject>

#include "openxr_meta.h"

#include "stardustvulkan.h"

class StardustOpenXR : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StardustVulkan *vulkan MEMBER vulkan)
public:
    explicit StardustOpenXR(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

    XrApplicationInfo *xrAppInfo = new XrApplicationInfo;
    XrInstanceCreateInfo *xrInstanceInfo = new XrInstanceCreateInfo;
    XrInstance *xrInstance = new XrInstance;
    XrSystemGetInfo *hmdInfo = new XrSystemGetInfo;
    XrSystemId *hmdID = new XrSystemId;
    XrSessionCreateInfo *xrSessionInfo = new XrSessionCreateInfo;
    XrSession *stardustSession = new XrSession;

    std::vector<char> *vulkanExtensionBuffer = new std::vector<char>;

    StardustVulkan *vulkan = nullptr;
    XrGraphicsBindingVulkanKHR vulkanBinding{XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR};

private:
    bool isExtensionSupported(char* extensionName, XrExtensionProperties* instanceExtensionProperties, uint32_t instanceExtensionCount);
};

#endif // STARDUSTOPENXR_H
