#include "stardustopenxrframe.h"

#include <QThread>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFramebufferObject>

namespace Stardust {

OpenXRGraphics::OpenXRGraphics(QObject *parent) : QObject(parent) {
}

OpenXRGraphics::~OpenXRGraphics() {
    xrDestroySwapchain(swapchains[0]);
    xrDestroySwapchain(swapchains[1]);
}

void OpenXRGraphics::preInitialize() {
    //Set the OpenGL version
    glFormat = new QSurfaceFormat;
    glFormat->setProfile(QSurfaceFormat::CoreProfile);
    glFormat->setRenderableType(QSurfaceFormat::OpenGL);
    glFormat->setMajorVersion(3);

    QSurfaceFormat::setDefaultFormat(*glFormat);

    //Create the OpenGL view rendering
    glContext = new QOpenGLContext(this);
    bool contextCreated = glContext->create();

//    //Check if external memory object extension is supported and if so import it
//    QByteArray ext_mem_obj_fd_name = QByteArrayLiteral("GL_EXT_memory_object_fd");
//    if(!glContext->hasExtension(ext_mem_obj_fd_name)) {
//        qDebug() << "Runtime does not support GL_EXT_memory_object_fd extension!" << endl;
////        std::abort();

//    } else {
//        qDebug() << "Runtime supports GL_EXT_memory_object_fd extension!" << endl;
//    }

//    QOpenGLExtension_

    //Create the offscreen surface
    surface = new QOffscreenSurface;
    surface->create();

    //Create QQuickRenderControl for both eyes
    quickRenderer = new QQuickRenderControl(this);
    window = new QQuickWindow(quickRenderer);
    window->setColor(Qt::transparent);


    window->setGeometry(QRect(QPoint(0,0),totalSize)); //Set the window bounds to the minimum to fit both views in case they are asymmetrical

    //Make the QML engine and components and so on
    qmlEngine = new QQmlEngine;
    if (!qmlEngine->incubationController())
        qmlEngine->setIncubationController(window->incubationController());

    //Make the context current
    bool isCurrent = glContext->makeCurrent(surface);

    //Create and link the FBO
    glFBO = new QOpenGLFramebufferObject(totalSize, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA8);
    window->setRenderTarget(glFBO);

    leftViewSize = eyeRects[0].size();
    rightViewSize = eyeRects[0].size();

    emit leftEyeSizeChanged();
    emit rightEyeSizeChanged();

    //Create QML component
    qmlComponent = new QQmlComponent(qmlEngine, "qrc:/core/StereoRender.qml", QQmlComponent::PreferSynchronous);

    //Load in the QML and add it to the window
    QObject *rootObject = qmlComponent->create();
    root = qobject_cast<QQuickItem *>(rootObject);
    root->setParentItem(window->contentItem());
    root->setPosition(QPoint(0, 0));
    root->setSize(totalSize);

    quickRenderer->initialize(glContext);
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

        openglImages[i].resize(swapchainImages[i].size());
        for (size_t j = 0; j < swapchainImages[i].size(); j++)
            openglImages[i][j] = swapchainImages[i][j].image;
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
