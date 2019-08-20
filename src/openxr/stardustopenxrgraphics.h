#ifndef STARDUSTOPENXRGRAPHICS_H
#define STARDUSTOPENXRGRAPHICS_H

#include <QVector3D>
#include <QQuaternion>
#include <QSize>
#include <QThread>

#include "openxr_meta.h"
#include "stardustopenxr.h"
#include <QtQuick3D/private/qquick3dviewport_p.h>

class StardustOpenXRFrame;

class StardustOpenXRGraphics : public QObject {
    Q_OBJECT

    Q_PROPERTY(StardustOpenXR *openxr MEMBER openxr)

    Q_PROPERTY(QSize eyeDimensions MEMBER eyeDimensions NOTIFY eyeDimensionsChanged)

    Q_PROPERTY(QQuick3DViewport *leftView MEMBER leftView)
    Q_PROPERTY(QQuick3DViewport *rightView MEMBER rightView)
    Q_PROPERTY(QQuick3DCamera *leftEye MEMBER leftEye)
    Q_PROPERTY(QQuick3DCamera *rightEye MEMBER rightEye)
public:
    explicit StardustOpenXRGraphics(QObject *parent = nullptr);
    ~StardustOpenXRGraphics();

    StardustOpenXR *openxr = nullptr;
    Q_INVOKABLE void initialize();

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

    QThread *frameThread = nullptr;
    StardustOpenXRFrame *frame = nullptr;

    QSize eyeDimensions = QSize(900, 900);
    std::vector<XrView> views;

    QQuick3DCamera *leftEye = nullptr;
    QQuick3DCamera *rightEye = nullptr;

    QQuick3DViewport *leftView = nullptr;
    QQuick3DViewport *rightView = nullptr;

    VkImage *leftEyeImage = VK_NULL_HANDLE;
    VkImage *rightEyeImage = VK_NULL_HANDLE;

    XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frameState{XR_TYPE_FRAME_STATE};

signals:
    void eyeDimensionsChanged();
    void startFrameLoop();
};

#endif // STARDUSTOPENXRGRAPHICS_H
