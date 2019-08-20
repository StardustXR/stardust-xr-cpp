#include "stardustopenxrgraphics.h"

#include <QDebug>
#include "stardustopenxrframe.h"

StardustOpenXRGraphics::StardustOpenXRGraphics(QObject *parent) : QObject(parent) {
}

StardustOpenXRGraphics::~StardustOpenXRGraphics() {
    xrDestroySwapchain(swapchains[0]);
    xrDestroySwapchain(swapchains[1]);
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

    //Set vulkan image's size
    imageExtent.height = eyeData[0].recommendedImageRectHeight;
    imageExtent.width = eyeData[0].recommendedImageRectWidth;

    //Update the eyeRect's extents
    eyeRect.extent.height = eyeData[0].recommendedImageRectHeight;
    eyeRect.extent.width = eyeData[0].recommendedImageRectWidth;

    //Update the QSize so the QML textures can be set
    eyeDimensions = QSize(eyeData[0].recommendedImageRectWidth, eyeData[0].recommendedImageRectHeight);
    emit eyeDimensionsChanged();

    //Get the amount of swapchain images
    xrEnumerateSwapchainImages(swapchains[0], 0, &swapchainImageCount, nullptr);

    //Add references to all swapchain images to swapchainImages
    leftSwapchainImages = std::vector<XrSwapchainImageVulkanKHR>(swapchainImageCount, swapchainImageTemplate);
    rightSwapchainImages = std::vector<XrSwapchainImageVulkanKHR>(swapchainImageCount, swapchainImageTemplate);

    //Add swapchain images to the array
    xrEnumerateSwapchainImages(swapchains[0], swapchainImageCount, nullptr, reinterpret_cast<XrSwapchainImageBaseHeader *>(leftSwapchainImages.data()));
    xrEnumerateSwapchainImages(swapchains[1], swapchainImageCount, nullptr, reinterpret_cast<XrSwapchainImageBaseHeader *>(rightSwapchainImages.data()));

    //Create a reference space relative to the STAGE (floor)
    xrCreateReferenceSpace(*openxr->stardustSession, &refSpaceInfo, &refSpace);

    //Initialize views vector
    views = std::vector<XrView>(2, XrView {XR_TYPE_VIEW, nullptr});

    //Create StardustOpenXRFrame object and add it to the thread
    frame = new StardustOpenXRFrame(this);
    frame->graphics = this;

    frameThread = new QThread(this);
    frame->moveToThread(frameThread);
    frame->thread = frameThread;

    connect(this, &StardustOpenXRGraphics::startFrameLoop, frame, &StardustOpenXRFrame::startFrame);
    frameThread->start();

//    emit startFrameLoop();
}
