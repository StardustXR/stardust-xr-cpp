#include "stardustopenxrgraphics.h"

#include "stardustopenxrframe.h"

namespace Stardust {

OpenXRGraphics::OpenXRGraphics(QObject *parent) : QObject(parent) {
}

OpenXRGraphics::~OpenXRGraphics() {
    xrDestroySwapchain(swapchains[0]);
    xrDestroySwapchain(swapchains[1]);
}

void OpenXRGraphics::preInitialize() {
}

void OpenXRGraphics::initialize() {
    //Get the stereo views' configuration properties
    xrGetViewConfigurationProperties(*openxr->xrInstance, *openxr->hmdID, openxr->viewConfig, &viewProperties);

    //Get the stereo views' configurations
    uint32_t viewCount = 2;
    xrEnumerateViewConfigurationViews(*openxr->xrInstance, *openxr->hmdID, openxr->viewConfig, viewCount, &viewCount, eyeData);

    //Do all this for both eyes
    for(int i=0; i<2; i++) {
        //Create the eyeRects
        eyeRects[i] = QRect();
        eyeRects[1].setX(eyeData[0].recommendedImageRectHeight + (eyeData[0].recommendedImageRectHeight % 2));

        //Update the eyeRects' extents
        eyeRects[i].setHeight(eyeData[i].recommendedImageRectHeight + (eyeData[i].recommendedImageRectHeight % 2));
        eyeRects[i].setWidth(eyeData[i].recommendedImageRectWidth + (eyeData[i].recommendedImageRectHeight % 2));
    }

    totalSize = QSize(
                eyeRects[0].width()+eyeRects[1].width(),
                std::max(eyeRects[0].height(), eyeRects[1].height())
            );

    //Update the swapchain info's values
    swapInfo.width = eyeRects[0].width();
    swapInfo.height = eyeRects[0].height();
    swapInfo.sampleCount = eyeData[0].recommendedSwapchainSampleCount;

    //Do all this for both eyes
    for(int i=0; i<2; i++) {
        //Create the swapchains
        xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[i]);

        //Get the amount of swapchain images
        uint32_t swapchainLength = 0;
        xrEnumerateSwapchainImages(swapchains[i], 0, &swapchainLength, nullptr);

        assert(swapchainLength > 0);

        // Resize the swapchain-length for current eye i
        swapchainImages[i].resize(swapchainLength);
        xrEnumerateSwapchainImages(swapchains[i], swapchainLength, &swapchainLength, reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchainImages[i].data()));

        vulkanImages[i].resize(swapchainImages[i].size());
        for (size_t j = 0; j < swapchainImages[i].size(); j++)
            vulkanImages[i][j] = swapchainImages[i][j].image;
    }

    //Create a reference space relative to the iz (floor)
    xrCreateReferenceSpace(*openxr->stardustSession, &refSpaceInfo, &refSpace);

    //Initialize views vector
    views = std::vector<XrView>(2, XrView {XR_TYPE_VIEW, nullptr});

    //Create OpenXRFrame object and add it to the thread
    frame = new OpenXRFrame(nullptr);
    frame->graphics = this;

    frameThread = new QThread(this);
    frame->thread = frameThread;

    //Start up the frame thread
    frameThread->start(QThread::TimeCriticalPriority);

    //Start the frame loop as soon as the scene graph initializes
    connect(this, &OpenXRGraphics::startFrameLoop, frame, &OpenXRFrame::initialize);
    emit startFrameLoop();
}

QSize OpenXRGraphics::getRightViewSize() const
{
    return rightViewSize;
}

QSize OpenXRGraphics::getLeftViewSize() const
{
    return leftViewSize;
}

QQuickWindow *OpenXRGraphics::getWindow() const {
    return window;
}

}
