#include "stardustopenxrgraphics.h"

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

    //Add views to the surfaces array
    surfaces[0] = leftView;
    surfaces[1] = rightView;

    //Update the swapchain info's values
    swapInfo.height = eyeData[0].recommendedImageRectHeight;
    swapInfo.width = eyeData[0].recommendedImageRectWidth;
    swapInfo.sampleCount = eyeData[0].recommendedSwapchainSampleCount;

    //Create the swapchains
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[0]);
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[1]);


    //Do all this for both eyes
    for(int i=0; i<2; i++) {
        //Update the OpenXR extents
        imageExtents[i].width = eyeData[i].recommendedImageRectWidth;
        imageExtents[i].height = eyeData[i].recommendedImageRectHeight;

        //Update the eyeRects' extents
        eyeRects[i].extent.height = eyeData[i].recommendedImageRectHeight;
        eyeRects[i].extent.width = eyeData[i].recommendedImageRectWidth;

        //Update the QSize so the QML textures can be set
        eyeDimensions = QSize(eyeData[i].recommendedImageRectWidth, eyeData[i].recommendedImageRectHeight);
        surfaces[i]->setWidth(eyeRects[0].extent.width);
        surfaces[i]->setHeight(eyeRects[0].extent.height);
        surfaces[i]->update();
        emit eyeDimensionsChanged();

        //Get the amount of swapchain images
        xrEnumerateSwapchainImages(swapchains[i], 0, &swapchainImageCount, nullptr);

        //Add references to all swapchain images to swapchainImages
        swapchainImages[i] = std::vector<XrSwapchainImageVulkanKHR>(swapchainImageCount, swapchainImageTemplate);

        //Add swapchain images to the array
        xrEnumerateSwapchainImages(swapchains[i], swapchainImageCount, nullptr, reinterpret_cast<XrSwapchainImageBaseHeader *>(swapchainImages[i].data()));

        //Add pointers to the swapchain images
        vulkanImages[i] = &swapchainImages[i][swapchainImageIndices[i]].image;
    }

    //Create a reference space relative to the iz (floor)
    xrCreateReferenceSpace(*openxr->stardustSession, &refSpaceInfo, &refSpace);

    //Initialize views vector
    views = std::vector<XrView>(2, XrView {XR_TYPE_VIEW, nullptr});

    //Create StardustOpenXRFrame object and add it to the thread
    frame = new StardustOpenXRFrame(nullptr);
    frame->graphics = this;

    frameThread = new QThread(this);
    frame->moveToThread(frameThread);
    frame->thread = frameThread;

    //Start up the frame loop
    connect(this, &StardustOpenXRGraphics::startFrameLoop, frame, &StardustOpenXRFrame::startFrame);
    frameThread->start();

    emit startFrameLoop();
}
