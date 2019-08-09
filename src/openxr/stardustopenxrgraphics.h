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
        XR_TYPE_SWAPCHAIN_CREATE_INFO,              //XrStructureType           type;
        nullptr,                                    //const void*               next;
        0,                                          //XrSwapchainCreateFlags    createFlags;
        XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT,    //XrSwapchainUsageFlags     usageFlags;
        VK_FORMAT_R8G8B8A8_SRGB,                    //int64_t                   format;
        1,                                          //uint32_t                  sampleCount;
        1,                                          //uint32_t                  width;
        1,                                          //uint32_t                  height;
        1,                                          //uint32_t                  faceCount;
        1,                                          //uint32_t                  arraySize;
        1                                           //uint32_t                  mipCount;
    };
    XrSwapchain swapchain = XR_NULL_HANDLE;
};

#endif // STARDUSTOPENXRGRAPHICS_H
