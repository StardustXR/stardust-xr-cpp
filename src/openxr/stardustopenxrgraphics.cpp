#include "stardustopenxrgraphics.h"

#include <QDebug>

StardustOpenXRGraphics::StardustOpenXRGraphics(QObject *parent) : QObject(parent) {
}

StardustOpenXRGraphics::~StardustOpenXRGraphics() {
    xrDestroySwapchain(swapchain);
    frameWorker->quit();
    frameWorker->wait();
}

void StardustOpenXRGraphics::initialize() {
    //Get the stereo views' configuration properties
    xrGetViewConfigurationProperties(*openxr->xrInstance, *openxr->hmdID, openxr->viewConfig, &viewProperties);

    //Get the stereo views' configurations
    xrEnumerateViewConfigurationViews(*openxr->xrInstance, *openxr->hmdID, openxr->viewConfig, 2, nullptr, eyeData);

    //Update the swapchain info's values
    swapInfo.height = eyeData[0].recommendedImageRectHeight;
    swapInfo.width = eyeData[0].recommendedImageRectWidth;
    swapInfo.sampleCount = eyeData[0].recommendedSwapchainSampleCount;

    //Create the swapchain
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchain);

    //Get the # of images
    xrEnumerateSwapchainImages(swapchain, 0, &swapchainImageCount, nullptr);

    //Set vulkan image's size
    vulkanImageTemplateInfo.extent.height = eyeData[0].recommendedImageRectHeight;
    vulkanImageTemplateInfo.extent.width = eyeData[0].recommendedImageRectWidth;

    //Add references to all swapchain images to swapchainImages
    swapchainImages = std::vector<XrSwapchainImageVulkanKHR>(swapchainImageCount, swapchainImageTemplate);

    //Update the swapchain images with the vulkan image
    for(XrSwapchainImageVulkanKHR &swapImage : swapchainImages) {
        vkCreateImage(openxr->vulkan->device, &vulkanImageTemplateInfo, nullptr, &swapImage.image);
    }

    //Set up the new thread to run the frame loop
    frameWorker = new StardustOpenXRFrameWorker(this);

    frameWorker->start();
}

void StardustOpenXRFrameWorker::run() {
    while (!QThread::currentThread()->isInterruptionRequested()) {
        StardustOpenXRGraphics *graphics = qobject_cast<StardustOpenXRGraphics *>(parent());

        //Wait for next frame
        xrWaitFrame(*graphics->openxr->stardustSession, &frameWaitInfo, &frameState);

        QThread::currentThread()->msleep(static_cast<unsigned long>(frameState.predictedDisplayTime*0.0000005));

        //Begin drawing a new frame
        xrBeginFrame(*graphics->openxr->stardustSession, nullptr);

        //Update the parent's FPS value
        qDebug() << "FPS:" << 1000000000/frameState.predictedDisplayTime;
        graphics->displayFPS = static_cast<uint>(1000000000/frameState.predictedDisplayTime);

        //Reset the maximum wait time for writing swapchain images
        swapImageWaitTime = 0;

        //Create acquire information
        XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO, nullptr};
        uint32_t index = 0;

        //Acquire the swapchain images
        xrAcquireSwapchainImage(graphics->swapchain, &acquireInfo, &index);

        //Do all this for both eyes
        for(XrSwapchainImageVulkanKHR &swapImage : graphics->swapchainImages) {
            //Create empty swapchain wait info
            XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};

            //Retrieve the wait info
            xrWaitSwapchainImage(graphics->swapchain, &waitInfo);

            //Get the maximum time to wait
            if(waitInfo.timeout > swapImageWaitTime) {
                swapImageWaitTime = waitInfo.timeout;
            }
        }

        //Wait for when the swapchain images are ready to be written
        if(swapImageWaitTime > 0)
            QThread::currentThread()->msleep(static_cast<unsigned long>(swapImageWaitTime*0.000001));


        //Create release information
        XrSwapchainImageReleaseInfo releaseInfo = {XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO, nullptr};

        //Release the swapchain image
        xrReleaseSwapchainImage(graphics->swapchain, &releaseInfo);

        //End the drawing of the current frame
        XrFrameEndInfo endInfo{XR_TYPE_FRAME_END_INFO};
        xrEndFrame(*graphics->openxr->stardustSession, &endInfo);
    }
}
