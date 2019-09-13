#include "stardustopenxrgraphics.h"

#include "stardustopenxrframe.h"

StardustOpenXRGraphics::StardustOpenXRGraphics(QObject *parent) : QObject(parent) {
}

StardustOpenXRGraphics::~StardustOpenXRGraphics() {
    xrDestroySwapchain(swapchains[0]);
    xrDestroySwapchain(swapchains[1]);
}

void StardustOpenXRGraphics::preInitialize() {
}

void StardustOpenXRGraphics::initialize() {
    //Get the stereo views' configuration properties
    xrGetViewConfigurationProperties(*openxr->xrInstance, *openxr->hmdID, openxr->viewConfig, &viewProperties);

    //Get the stereo views' configurations
    uint32_t viewCount = 2;
    xrEnumerateViewConfigurationViews(*openxr->xrInstance, *openxr->hmdID, openxr->viewConfig, viewCount, &viewCount, eyeData);

    //Update the swapchain info's values
    swapInfo.height = eyeData[0].recommendedImageRectHeight;
    swapInfo.width = eyeData[0].recommendedImageRectWidth;
    swapInfo.sampleCount = eyeData[0].recommendedSwapchainSampleCount;

    //Create the swapchains
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[0]);
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[1]);

    imageOffsets[1].x = eyeData[0].recommendedImageRectWidth;

    //Do all this for both eyes
    for(int i=0; i<2; i++) {
        //Update the OpenXR extents
        imageExtents[i].width = eyeData[i].recommendedImageRectWidth;
        imageExtents[i].height = eyeData[i].recommendedImageRectHeight;

        //Update the eyeRects' extents
        eyeRects[i].extent.height = eyeData[i].recommendedImageRectHeight;
        eyeRects[i].extent.width = eyeData[i].recommendedImageRectWidth;

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

    //Create StardustOpenXRFrame object and add it to the thread
    frame = new StardustOpenXRFrame(nullptr);
    frame->graphics = this;

    frameThread = new QThread(this);
    frame->thread = frameThread;

    //Start up the frame thread
    frameThread->start();

    //Start the frame loop as soon as the scene graph initializes
    connect(this, &StardustOpenXRGraphics::startFrameLoop, frame, &StardustOpenXRFrame::initialize);
    emit startFrameLoop();
}

QSize StardustOpenXRGraphics::getRightViewSize() const
{
    return rightViewSize;
}

QSize StardustOpenXRGraphics::getLeftViewSize() const
{
    return leftViewSize;
}

QQuickWindow *StardustOpenXRGraphics::getWindow() const {
    return window;
}
