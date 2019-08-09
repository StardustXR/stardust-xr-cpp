#include "stardustopenxr.h"

#include <iostream>
#include <QDebug>
#include <signal.h>

StardustOpenXR::StardustOpenXR(QObject *parent) : QObject(parent) {
}

void StardustOpenXR::initialize() {
    //XrApplicationInfo *xrAppInfo
    strcpy(xrAppInfo->applicationName, QString("Stardust XR").toUtf8());
    xrAppInfo->applicationVersion = 1;
    strcpy(xrAppInfo->engineName, QString("Qt").toUtf8());
    xrAppInfo->engineVersion = 5;
    xrAppInfo->apiVersion = XR_CURRENT_API_VERSION;

    uint32_t extensionCount = 0;
    xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr);

    printf("Runtime supports %d extensions\n", extensionCount);

    XrExtensionProperties extensionProperties[extensionCount];
    for (uint16_t i = 0; i < extensionCount; i++) {
        // we usually have to fill in the type (for validation) and set
        // next to NULL (or a pointer to an extension specific struct)
        extensionProperties[i].type = XR_TYPE_EXTENSION_PROPERTIES;
        extensionProperties[i].next = nullptr;
    }

    xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties);

    if (!isExtensionSupported(XR_KHR_VULKAN_ENABLE_EXTENSION_NAME, extensionProperties, extensionCount)) {
        qDebug() << "Runtime does not support Vulkan extension!" << endl;
    }

    const char* const enabledExtensions[] = {XR_KHR_VULKAN_ENABLE_EXTENSION_NAME};

    //XrInstanceCreateInfo *xrInstanceInfoXrInstanceCreateInfo *xrInstanceInfo
    xrInstanceInfo->type = XR_TYPE_INSTANCE_CREATE_INFO;
    xrInstanceInfo->next = nullptr;
    xrInstanceInfo->createFlags = 0;
    xrInstanceInfo->applicationInfo = *xrAppInfo;
    xrInstanceInfo->enabledApiLayerCount = 0;
    xrInstanceInfo->enabledExtensionCount = 1;
    xrInstanceInfo->enabledExtensionNames = enabledExtensions;

    //Create the OpenXR Instance
    xrCreateInstance(xrInstanceInfo, xrInstance);

    //XrSystemGetInfo
    hmdInfo->type = XR_TYPE_SYSTEM_GET_INFO;
    hmdInfo->next = nullptr;
    hmdInfo->formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    //XrSystemId *hmdID
    xrGetSystem(*xrInstance, hmdInfo, hmdID);

    //Initialize Vulkan
    vulkan->initialize();

    //Bind vulkan to this OpenXR session
    //XrGraphicsBindingVulkanKHR *vulkanBinding
    vulkanBinding->type = XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR;
    vulkanBinding->next = nullptr;
    vulkanBinding->instance = *vulkan->instance;
    vulkanBinding->physicalDevice = vulkan->physicalDevice;
    vulkanBinding->device = *vulkan->device;
    vulkanBinding->queueFamilyIndex = vulkan->queueFamilyIndex;
    vulkanBinding->queueIndex = vulkan->queueIndex;

    //XrSessionCreateInfo *xrSessionInfo
    xrSessionInfo->type = XR_TYPE_SESSION_CREATE_INFO;
    xrSessionInfo->next = vulkanBinding;
    xrSessionInfo->systemId = *hmdID;
    xrSessionInfo->createFlags = 0;

    //XrSession *stardustSession;
    xrCreateSession(*xrInstance, xrSessionInfo, stardustSession);
}

bool StardustOpenXR::isExtensionSupported(char *extensionName, XrExtensionProperties *instanceExtensionProperties, uint32_t instanceExtensionCount) {
    for (uint32_t supportedIndex = 0; supportedIndex < instanceExtensionCount; supportedIndex++) {
        if (!strcmp(extensionName,
                    instanceExtensionProperties[supportedIndex].extensionName)) {
            return true;
        }
    }
    return false;
}
