#include "stardustopenxrgraphics.h"

#include <QDebug>

StardustOpenXRGraphics::StardustOpenXRGraphics(QObject *parent) : QObject(parent) {
}

StardustOpenXRGraphics::~StardustOpenXRGraphics() {
    xrDestroySwapchain(swapchains[0]);
    xrDestroySwapchain(swapchains[1]);
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

    //Create the swapchains
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[0]);
    xrCreateSwapchain(*openxr->stardustSession, &swapInfo, &swapchains[1]);

    //Set vulkan image's size
    vulkanImageTemplateInfo.extent.height = eyeData[0].recommendedImageRectHeight;
    vulkanImageTemplateInfo.extent.width = eyeData[0].recommendedImageRectWidth;

    //Update the eyeRect's extents
    eyeRect.extent.height = eyeData[0].recommendedImageRectHeight;
    eyeRect.extent.width = eyeData[0].recommendedImageRectWidth;

    //Update the QSize so the QML textures can be set
    eyeDimensions = QSize(eyeData[0].recommendedImageRectWidth, eyeData[0].recommendedImageRectHeight);
    emit eyeDimensionsChanged();

    //Add references to all swapchain images to swapchainImages
    leftSwapchainImages = std::vector<XrSwapchainImageVulkanKHR>(swapchainImageCount, swapchainImageTemplate);
    rightSwapchainImages = std::vector<XrSwapchainImageVulkanKHR>(swapchainImageCount, swapchainImageTemplate);

    //Add swapchain images to the array
    xrEnumerateSwapchainImages(swapchains[0], swapchainImageCount, nullptr, reinterpret_cast<XrSwapchainImageBaseHeader *>(leftSwapchainImages.data()));
    xrEnumerateSwapchainImages(swapchains[0], swapchainImageCount, nullptr, reinterpret_cast<XrSwapchainImageBaseHeader *>(rightSwapchainImages.data()));

    //Create a reference space relative to the STAGE (floor)
    xrCreateReferenceSpace(*openxr->stardustSession, &refSpaceInfo, &refSpace);

    //Initialize views vector
    views = std::vector<XrView>(2, XrView {XR_TYPE_VIEW, nullptr});

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
//        qDebug() << "FPS:" << 1000000000/frameState.predictedDisplayTime;
        graphics->displayFPS = static_cast<uint>(1000000000/frameState.predictedDisplayTime);

        //Reset the maximum wait time for writing swapchain images
        swapImageWaitTime = 0;

        //Do all this for both eyes
        for(int i=0; i<2; i++) {
            //Create acquire information
            XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO, nullptr};

            //Grab the swapchain image
            xrAcquireSwapchainImage(graphics->swapchains[i], &acquireInfo, &graphics->swapchainImageIndices.at(i));

            //Create empty swapchain wait info
            XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, nullptr};

            //Retrieve the wait info
            xrWaitSwapchainImage(graphics->swapchains[i], &waitInfo);

            //Get the maximum time to wait
            if(waitInfo.timeout > swapImageWaitTime) {
                swapImageWaitTime = waitInfo.timeout;
            }
        }

        //Wait for when the swapchain images are ready to be written
        if(swapImageWaitTime > 0)
            QThread::currentThread()->msleep(static_cast<unsigned long>(swapImageWaitTime*0.000001));

        //Update view information
        graphics->viewLocateInfo.viewConfigurationType = graphics->openxr->viewConfig;
        graphics->viewLocateInfo.displayTime = frameState.predictedDisplayTime;
        graphics->viewLocateInfo.space = graphics->refSpace;

        //Locate views
        xrLocateViews(*graphics->openxr->stardustSession, &graphics->viewLocateInfo, &graphics->viewState, 2, nullptr, graphics->views.data());

        //Do for each eye
        for(int i=0; i<2; i++) {
            Qt3DRender::QCamera *eye = graphics->leftEye;
            if(i==1)
                eye = graphics->rightEye;

            XrView &view = graphics->views.at(i);
            //Set the cameras' positon to the pose position
            QVector3D position(view.pose.position.x, view.pose.position.z, view.pose.position.y);
            eye->setPosition(position);

            //Set the cameras' orientation to match the orientation
            QQuaternion rotation = QQuaternion(view.pose.orientation.w, view.pose.orientation.x, view.pose.orientation.y, view.pose.orientation.z);
            QVector3D defaultCameraOrientation = QVector3D(0,-1,0);
            eye->setViewCenter(eye->position()+(rotation*defaultCameraOrientation));

            //Update properties on the XrFrameEndInfo and its dependencies
            graphics->stardustLayerViews[i].fov = view.fov;
            graphics->stardustLayerViews[i].pose = view.pose;
            graphics->stardustLayerViews[i].subImage = XrSwapchainSubImage {
                graphics->swapchains[i],
                graphics->eyeRect,
                graphics->swapchainImageIndices[i]
            };
        }

        //Update properties on the XrFrameEndInfo and its dependencies
        XrCompositionLayerProjection stardustLayer = {
            XR_TYPE_COMPOSITION_LAYER_PROJECTION,
            nullptr,
            0,
            graphics->refSpace,
            2,
            graphics->stardustLayerViews
        };
        stardustLayer.views = graphics->stardustLayerViews;
        std::vector<XrCompositionLayerBaseHeader*> layers;
        if(frameState.shouldRender) {
            layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&stardustLayer));
        }
        XrFrameEndInfo endInfo = {
            XR_TYPE_FRAME_END_INFO,
            nullptr,
            graphics->frameWorker->frameState.predictedDisplayTime,
            XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
            static_cast<uint32_t>(layers.size()),
            layers.data()
        };

        //Create release information
        XrSwapchainImageReleaseInfo releaseInfo = {XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO, nullptr};

        //Release the swapchain images
        xrReleaseSwapchainImage(graphics->swapchains[0], &releaseInfo);
        xrReleaseSwapchainImage(graphics->swapchains[1], &releaseInfo);

        //End the drawing of the current frame
        xrEndFrame(*graphics->openxr->stardustSession, &endInfo);
    }
}
