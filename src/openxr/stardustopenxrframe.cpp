#include "stardustopenxrframe.h"

#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QQuickRenderControl>

#include <QtQuick3D/private/qquick3dcamera_p.h>
#include <QDebug>

#define RAD2DEG 180/3.14159

namespace Stardust {

OpenXRFrame::OpenXRFrame(QObject *parent) : QObject(parent) {
}

void OpenXRFrame::initialize() {
    bool isCurrent = graphics->glContext->makeCurrent(graphics->surface);

//    createEXTBuffers();

	frameTimer = new QElapsedTimer;

    emit graphics->openxr->ready();

    for(;;) {
        startFrame();
        renderFrame();
        endFrame();
    }
}

void OpenXRFrame::startFrame() {
//    qDebug() << "Starting frame";
	frameTimer->start();

    //Wait for next frame
    xrWaitFrame(*graphics->openxr->stardustSession, &graphics->frameWaitInfo, &graphics->frameState);

    //Begin drawing a new frame
    xrBeginFrame(*graphics->openxr->stardustSession, nullptr);

    //Update the parent's FPS value
//        qDebug() << "FPS:" << 1000000000/frameState.predictedDisplayTime;
    graphics->displayFPS = static_cast<uint>(1000000000/graphics->frameState.predictedDisplayTime);

    //Update view information
    graphics->viewLocateInfo.viewConfigurationType = graphics->openxr->viewConfig;
    graphics->viewLocateInfo.displayTime = graphics->frameState.predictedDisplayTime;
    graphics->viewLocateInfo.space = graphics->refSpace;

    //Locate views
    xrLocateViews(*graphics->openxr->stardustSession, &graphics->viewLocateInfo, &graphics->viewState, 2, nullptr, graphics->views.data());

    //Do for each eye
    for(int i=0; i<2; i++) {
        //Grab the swapchain image
        xrAcquireSwapchainImage(graphics->swapchains[i], &acquireInfo, &graphics->swapchainImageIndices[i]);

        //Wait for when the swapchain images are ready to be written
        xrWaitSwapchainImage(graphics->swapchains[i], &waitInfo);

        //Get the current view (makes code neater)
        XrView &view = graphics->views[i];

        //Get the current eye camera (makes code neater)
        QQuick3DCamera *eye = qobject_cast<QQuick3DCamera *>(graphics->leftEye);
        if(i==1)
            eye = qobject_cast<QQuick3DCamera *>(graphics->rightEye);

        if(!eye)
            break;

        //Set the cameras' positon to the pose position
        QVector3D position = QVector3D(
            -view.pose.position.x,
            -view.pose.position.y,
            -view.pose.position.z
        );
        eye->setPosition(position);

        //Set the cameras' orientation to match the orientation
        QQuaternion rotation = QQuaternion(
            view.pose.orientation.w,
            view.pose.orientation.x,
            view.pose.orientation.y,
            view.pose.orientation.z
        );
        QVector3D euler = rotation.toEulerAngles();

        eye->setRotation(QVector3D(
            euler.x(),
            -euler.y(),
            -euler.z()
        ));

//        eye->setIsFieldOfViewHorizontal(true);
//        eye->setFieldOfView((view.fov.angleRight-view.fov.angleLeft)*RAD2DEG);

        eye->setFrustumTop      (std::sin(view.fov.angleUp)*eye->clipNear());
        eye->setFrustumBottom   (std::sin(view.fov.angleDown)*eye->clipNear());
        eye->setFrustumLeft     (std::sin(view.fov.angleLeft)*eye->clipNear());
        eye->setFrustumRight    (std::sin(view.fov.angleRight)*eye->clipNear());

        //Update properties on the XrFrameEndInfo and its dependencies
        graphics->stardustLayerViews[i].fov = view.fov;
        graphics->stardustLayerViews[i].pose = view.pose;
        graphics->stardustLayerViews[i].subImage = XrSwapchainSubImage {
            graphics->swapchains[i],
            XrRect2Di {
                XrOffset2Di {0, 0},
                XrExtent2Di {
                    graphics->eyeRects[i].width(),
                    graphics->eyeRects[i].height()
                }
            },
            graphics->swapchainImageIndices[i]
        };
    }
}

void OpenXRFrame::renderFrame() {
//    qDebug() << "Rendering frame";

    graphics->quickRenderer->polishItems();
    graphics->quickRenderer->sync();
    graphics->quickRenderer->render();

//    graphics->glFBO->toImage().save("out.png", 0, 10);

    (reinterpret_cast<PFNGLBINDFRAMEBUFFEREXTPROC>(graphics->glContext->getProcAddress("glBindFramebufferEXT")))(
        GL_FRAMEBUFFER_EXT, graphics->glFBO->handle()
    );

    (reinterpret_cast<PFNGLCLIPCONTROLPROC>(graphics->glContext->getProcAddress("glClipControl")))(
        GL_UPPER_LEFT, GL_ZERO_TO_ONE
    );

    (reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DEXTPROC>(graphics->glContext->getProcAddress("glFramebufferTexture2DEXT")))(
        GL_FRAMEBUFFER_EXT,
        GL_COLOR_ATTACHMENT0_EXT,
        GL_TEXTURE_2D,
        colorTex,
        0
    );

    glFinish();

//    copyFrame(0);
//    copyFrame(1);
}

void OpenXRFrame::endFrame() {
//    qDebug() << "Ending frame";
    //Update properties on the XrFrameEndInfo and its dependencies
    XrCompositionLayerProjection stardustLayer = {
        XR_TYPE_COMPOSITION_LAYER_PROJECTION,
        nullptr,
        0,
        graphics->refSpace,
        2,
        graphics->stardustLayerViews
    };
    std::vector<XrCompositionLayerBaseHeader*> layers;

    layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&stardustLayer));

    XrFrameEndInfo endInfo = {
        XR_TYPE_FRAME_END_INFO,
        nullptr,
        graphics->frameState.predictedDisplayTime,
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

	fps = 1000/frameTimer->elapsed();
//    qDebug() << "FPS: " << fps << endl;
}

}
