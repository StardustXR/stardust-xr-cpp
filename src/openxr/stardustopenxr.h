#ifndef STARDUSTOPENXR_H
#define STARDUSTOPENXR_H

#include "stardustopengl.h"
namespace Stardust {

class OpenXR : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OpenGL *opengl MEMBER opengl)
public:
    explicit OpenXR(QObject *parent = nullptr);
    ~OpenXR();

    OpenGL *opengl = nullptr;
    Q_INVOKABLE void initialize();

    XrApplicationInfo *xrAppInfo = new XrApplicationInfo;
    XrInstanceCreateInfo *xrInstanceInfo = new XrInstanceCreateInfo;
    XrInstance *xrInstance = new XrInstance;
    XrSystemGetInfo *hmdInfo = new XrSystemGetInfo;
    XrSystemId *hmdID = new XrSystemId;
    XrSessionCreateInfo *xrSessionInfo = new XrSessionCreateInfo;
    XrSession *stardustSession = new XrSession;

    XrGraphicsRequirementsOpenGLKHR graphicsRequirements{XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR};

    std::vector<char> *openglExtensionBuffer = new std::vector<char>;

//    Xr vulkanBinding{XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR};
    XrGraphicsBindingOpenGLXlibKHR openglBinding{XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR};

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

}

#endif // STARDUSTOPENXR_H
