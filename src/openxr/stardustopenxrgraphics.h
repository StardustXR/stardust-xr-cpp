#ifndef STARDUSTOPENXRGRAPHICS_H
#define STARDUSTOPENXRGRAPHICS_H

#include <QSize>
#include <QThread>

#include <Qt3DRender>
#include <QScreen>
#include <QOffscreenSurface>

#include "openxr_meta.h"
#include "stardustopenxr.h"

class StardustOpenXRFrameWorker : public QThread {
    Q_OBJECT
public:
    StardustOpenXRFrameWorker(QObject *parent = nullptr) : QThread(parent) {}

    XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frameState{XR_TYPE_FRAME_STATE};
    long swapImageWaitTime = 0;
signals:
    void renderFrame(float dt);
private:
    void run() override;
};

class StardustOpenXRGraphics : public QObject {
    Q_OBJECT

    Q_PROPERTY(StardustOpenXR *openxr MEMBER openxr)

    Q_PROPERTY(QSize eyeDimensions MEMBER eyeDimensions NOTIFY eyeDimensionsChanged)

    Q_PROPERTY(Qt3DRender::QCamera *leftEye MEMBER leftEye)
    Q_PROPERTY(Qt3DRender::QCamera *rightEye MEMBER rightEye)
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
        XR_TYPE_SWAPCHAIN_CREATE_INFO,                                                      //XrStructureType           type;
        nullptr,                                                                            //const void*               next;
        0,                                                                                  //XrSwapchainCreateFlags    createFlags;
        XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT | XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT,      //XrSwapchainUsageFlags     usageFlags;
        VK_FORMAT_R8G8B8A8_SRGB,                                                            //int64_t                   format;
        1,                                                                                  //uint32_t                  sampleCount;
        1,                                                                                  //uint32_t                  width;
        1,                                                                                  //uint32_t                  height;
        1,                                                                                  //uint32_t                  faceCount;
        1,                                                                                  //uint32_t                  arraySize;
        1                                                                                   //uint32_t                  mipCount;
    };
    XrSwapchain swapchains[2] = {
        XrSwapchain {},
        XrSwapchain {}
    };

    VkExtent3D imageExtent = {0, 0, 1};

    uint32_t swapchainImageCount = 0;
    XrSwapchainImageVulkanKHR swapchainImageTemplate = {
        XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR,
        nullptr,
        VkImage()
    };
    std::vector<XrSwapchainImageVulkanKHR> leftSwapchainImages;
    std::vector<XrSwapchainImageVulkanKHR> rightSwapchainImages;

    std::vector<uint32_t> swapchainImageIndices = std::vector<uint32_t>(2);

    XrReferenceSpaceCreateInfo refSpaceInfo = {
        XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
        nullptr,
        XR_REFERENCE_SPACE_TYPE_STAGE,
        XrPosef {
            XrQuaternionf {0.0f,0.0f,1.0f,0.0f},
            XrVector3f {0.0f,0.0f,0.0f}
        }
    };
    XrSpace refSpace = XR_NULL_HANDLE;

    XrViewLocateInfo viewLocateInfo = {
        XR_TYPE_VIEW_LOCATE_INFO,
        nullptr
    };
    XrViewState viewState = {
        XR_TYPE_VIEW_STATE,
        nullptr
    };

    XrRect2Di eyeRect = {
        XrOffset2Di {0,0},
        XrExtent2Di {1,1}
    };

    XrCompositionLayerProjectionView stardustLayerViews[2] = {
        XrCompositionLayerProjectionView {
            XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW,
            nullptr
        },
        XrCompositionLayerProjectionView {
            XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW,
            nullptr
        }
    };

    QSize eyeDimensions;
    std::vector<XrView> views;
    Qt3DRender::QCamera *leftEye = nullptr;
    Qt3DRender::QCamera *rightEye = nullptr;

    VkImage *leftEyeImage;
    VkImage *rightEyeImage;

signals:
    void eyeDimensionsChanged();
};

#endif // STARDUSTOPENXRGRAPHICS_H
