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




        //End the drawing of the current frame
        XrFrameEndInfo endInfo{XR_TYPE_FRAME_END_INFO};
        xrEndFrame(*graphics->openxr->stardustSession, &endInfo);
    }
}
