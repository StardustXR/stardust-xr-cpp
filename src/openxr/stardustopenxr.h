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
    ~StardustOpenXR();

    StardustVulkan *vulkan = nullptr;
    Q_INVOKABLE void initialize();

    XrApplicationInfo *xrAppInfo = new XrApplicationInfo;
    XrInstanceCreateInfo *xrInstanceInfo = new XrInstanceCreateInfo;
    XrInstance *xrInstance = new XrInstance;
    XrSystemGetInfo *hmdInfo = new XrSystemGetInfo;
    XrSystemId *hmdID = new XrSystemId;
    XrSessionCreateInfo *xrSessionInfo = new XrSessionCreateInfo;
    XrSession *stardustSession = new XrSession;

    std::vector<char> *vulkanExtensionBuffer = new std::vector<char>;

    XrGraphicsBindingVulkanKHR vulkanBinding{XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR};

    XrViewConfigurationType viewConfig{XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO};

    XrSessionBeginInfo beginInfo{
        XR_TYPE_SESSION_BEGIN_INFO,
                nullptr,
                viewConfig
    };

signals:
    void ready();

private:
    bool isExtensionSupported(char* extensionName, XrExtensionProperties* instanceExtensionProperties, uint32_t instanceExtensionCount);
};

#endif // STARDUSTOPENXR_H
