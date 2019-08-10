#ifndef STARDUSTOPENXRGRAPHICS_H
#define STARDUSTOPENXRGRAPHICS_H

#include <QObject>
#include <QThread>

#include "openxr_meta.h"
#include "stardustopenxr.h"

class StardustOpenXRFrameWorker : public QThread {
    Q_OBJECT
public:
    StardustOpenXRFrameWorker(QObject *parent = nullptr) : QThread(parent) {}
private:
    void run() override;
    XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frameState{XR_TYPE_FRAME_STATE};
    long swapImageWaitTime = 0;
};

class StardustOpenXRGraphics : public QObject {
    Q_OBJECT

    Q_PROPERTY(StardustOpenXR *openxr MEMBER openxr)
public:
    explicit StardustOpenXRGraphics(QObject *parent = nullptr);
    ~StardustOpenXRGraphics();

    StardustOpenXR *openxr = nullptr;
    Q_INVOKABLE void initialize();

    StardustOpenXRFrameWorker *frameWorker;
    uint displayFPS = 0;

    XrViewConfigurationProperties viewProperties{XR_TYPE_VIEW_CONFIGURATION_PROPERTIES};
    XrViewConfigurationView eyeData[2];

    XrSwapchainCreateInfo swapInfo = {
        XR_TYPE_SWAPCHAIN_CREATE_INFO,                  //XrStructureType           type;
        nullptr,                                        //const void*               next;
        0,                                              //XrSwapchainCreateFlags    createFlags;
        XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT,        //XrSwapchainUsageFlags     usageFlags;
        VK_FORMAT_R8G8B8A8_SRGB,                        //int64_t                   format;
        1,                                              //uint32_t                  sampleCount;
        1,                                              //uint32_t                  width;
        1,                                              //uint32_t                  height;
        1,                                              //uint32_t                  faceCount;
        1,                                              //uint32_t                  arraySize;
        1                                               //uint32_t                  mipCount;
    };
    XrSwapchain swapchain = XR_NULL_HANDLE;

    VkImageCreateInfo vulkanImageTemplateInfo = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,            //VkStructureType          sType;
        nullptr,                                        //const void*              pNext;
        VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT,        //VkImageCreateFlags       flags;
        VK_IMAGE_TYPE_2D,                               //VkImageType              imageType;
        VK_FORMAT_R8G8B8A8_SRGB,                        //VkFormat                 format;
        VkExtent3D{0, 0, 3},                            //VkExtent3D               extent;
        1,                                              //uint32_t                 mipLevels;
        1,                                              //uint32_t                 arrayLayers;
        VK_SAMPLE_COUNT_1_BIT,                          //VkSampleCountFlagBits    samples;
        VK_IMAGE_TILING_OPTIMAL,                        //VkImageTiling            tiling;
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,    //VkImageUsageFlags        usage;
        VK_SHARING_MODE_EXCLUSIVE,                      //VkSharingMode            sharingMode;
        1,                                              //uint32_t                 queueFamilyIndexCount;
        nullptr,                                        //const uint32_t*          pQueueFamilyIndices;
        VK_IMAGE_LAYOUT_UNDEFINED                       //VkImageLayout            initialLayout;
    };

    uint32_t swapchainImageCount = 0;
    XrSwapchainImageVulkanKHR swapchainImageTemplate = {
        XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR,
        nullptr,
        VkImage()
    };
    std::vector<XrSwapchainImageVulkanKHR> swapchainImages;
};

#endif // STARDUSTOPENXRGRAPHICS_H
