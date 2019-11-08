#include "stardustopenxr.h"

#include <iostream>
#include <QDebug>
#include <signal.h>

namespace Stardust {

OpenXR::OpenXR(QObject *parent) : QObject(parent) {
}

OpenXR::~OpenXR() {
    xrEndSession(*stardustSession);
    xrDestroySession(*stardustSession);
    xrDestroyInstance(*xrInstance);
}

void OpenXR::initialize() {
    strcpy(xrAppInfo->applicationName, QString("Stardust XR").toUtf8());
    xrAppInfo->applicationVersion = 1;
    strcpy(xrAppInfo->engineName, QString("Qt").toUtf8());
    xrAppInfo->engineVersion = 5;
    xrAppInfo->apiVersion = XR_CURRENT_API_VERSION;

    uint32_t extensionCount = 0;
    xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr);

    qDebug() << "Runtime supports" << extensionCount << "extensions\n" << extensionCount;

    XrExtensionProperties extensionProperties[extensionCount];
    for (uint16_t i = 0; i < extensionCount; i++) {
        // we usually have to fill in the type (for validation) and set
        // next to NULL (or a pointer to an extension specific struct)
        extensionProperties[i].type = XR_TYPE_EXTENSION_PROPERTIES;
        extensionProperties[i].next = nullptr;
    }

    xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties);

    if (!isExtensionSupported(XR_KHR_OPENGL_ENABLE_EXTENSION_NAME, extensionProperties, extensionCount)) {
        qDebug() << "Runtime does not support OpenGL extension!" << endl;
    }

    const char* const enabledExtensions[] = {XR_KHR_OPENGL_ENABLE_EXTENSION_NAME};

    //XrInstanceCreateInfo *xrInstanceInfoXrInstanceCreateInfo *xrInstanceInfo
    xrInstanceInfo->type = XR_TYPE_INSTANCE_CREATE_INFO;
    xrInstanceInfo->next = nullptr;
    xrInstanceInfo->createFlags = 0;
    xrInstanceInfo->applicationInfo = *xrAppInfo;
    xrInstanceInfo->enabledApiLayerCount = 0;
    xrInstanceInfo->enabledExtensionCount = 1;
    xrInstanceInfo->enabledExtensionNames = enabledExtensions;

    //Create the OpenXR Instance
    XrResult xrResult = xrCreateInstance(xrInstanceInfo, xrInstance);

    //XrSystemGetInfo
    hmdInfo->type = XR_TYPE_SYSTEM_GET_INFO;
    hmdInfo->next = nullptr;
    hmdInfo->formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    //XrSystemId *hmdID
    xrGetSystem(*xrInstance, hmdInfo, hmdID);

    xrGetOpenGLGraphicsRequirementsKHR(*xrInstance, *hmdID, &graphicsRequirements);

    //Initialize Vulkan
    opengl->initialize();

    //Bind vulkan to this OpenXR session
    //XrGraphicsBindingOpenGLXlibKHR openglBinding
    openglBinding.xDisplay = opengl->display;
    openglBinding.glxContext = opengl->context;
    openglBinding.glxDrawable = opengl->drawable;
    openglBinding.glxFBConfig = *opengl->framebufferConfig;

    //XrSessionCreateInfo *xrSessionInfo
    xrSessionInfo->type = XR_TYPE_SESSION_CREATE_INFO;
    xrSessionInfo->next = const_cast<const XrGraphicsBindingOpenGLXlibKHR*>(&openglBinding);
    xrSessionInfo->systemId = *hmdID;
    xrSessionInfo->createFlags = 0;

    //XrSession *stardustSession;
    xrCreateSession(*xrInstance, xrSessionInfo, stardustSession);

    //Start the XrSession
    xrBeginSession(*stardustSession, &beginInfo);
}

bool OpenXR::isExtensionSupported(char *extensionName, XrExtensionProperties *instanceExtensionProperties, uint32_t instanceExtensionCount) {
    for (uint32_t supportedIndex = 0; supportedIndex < instanceExtensionCount; supportedIndex++) {
        if (!strcmp(extensionName,
                    instanceExtensionProperties[supportedIndex].extensionName)) {
            return true;
        }
    }
    return false;
}

}
