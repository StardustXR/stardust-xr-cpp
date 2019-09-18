#include <GL/glew.h>
#include <GL/gl.h>

#include "stardustopenxrframe.h"
#include "qopenglfunctions.h"
#include <QtQuick3D/private/qquick3dcamera_p.h>
#include <QDebug>

#define RAD2DEG 180/3.14159

namespace Stardust {

OpenXRFrame::OpenXRFrame(QObject *parent) : QObject(parent) {
    connect(this, &OpenXRFrame::initialized, this, &OpenXRFrame::startFrame);

    connect(this, &OpenXRFrame::startedFrame, this, &OpenXRFrame::renderFrame);
    connect(this, &OpenXRFrame::renderedFrame, this, &OpenXRFrame::endFrame);
    connect(this, &OpenXRFrame::frameEnded, this, &OpenXRFrame::startFrame);
}

void OpenXRFrame::initialize() {
    initRenderControl();

    bool isCurrent = graphics->glContext->makeCurrent(graphics->surface);

    createEXTBuffers();

	frameTimer = new QElapsedTimer;

    emit initialized();
    emit graphics->openxr->ready();
}

void OpenXRFrame::initRenderControl() {

    //Create the OpenGL view rendering
    graphics->glContext = new QOpenGLContext(this);
    bool contextCreated = graphics->glContext->create();

    //Create the offscreen surface
    graphics->surface = new QOffscreenSurface;
    graphics->surface->create();

    //Create QQuickRenderControl for both eyes
    graphics->quickRenderer = new QQuickRenderControl(this);
    graphics->window = new QQuickWindow(graphics->quickRenderer);
    graphics->window->setColor(Qt::transparent);


    graphics->window->setGeometry(QRect(QPoint(0,0),graphics->totalSize)); //Set the window bounds to the minimum to fit both views in case they are asymmetrical

    //Make the QML engine and components and so on
    graphics->qmlEngine = new QQmlEngine;
    if (!graphics->qmlEngine->incubationController())
        graphics->qmlEngine->setIncubationController(graphics->window->incubationController());

    //Make the context current
    bool isCurrent = graphics->glContext->makeCurrent(graphics->surface);

    //Create and link the FBO
    graphics->glFBO = new QOpenGLFramebufferObject(graphics->totalSize, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA8);
    graphics->window->setRenderTarget(graphics->glFBO);

    graphics->leftViewSize = graphics->eyeRects[0].size();
    graphics->rightViewSize = graphics->eyeRects[0].size();

    emit graphics->leftEyeSizeChanged();
    emit graphics->rightEyeSizeChanged();

    //Create QML component
    graphics->qmlComponent = new QQmlComponent(graphics->qmlEngine, "qrc:/core/StereoRender.qml", QQmlComponent::PreferSynchronous);

    //Load in the QML and add it to the window
    QObject *rootObject = graphics->qmlComponent->create();
    graphics->root = qobject_cast<QQuickItem *>(rootObject);
    graphics->root->setParentItem(graphics->window->contentItem());
    graphics->root->setPosition(QPoint(0, 0));
    graphics->root->setSize(graphics->totalSize);

    emit graphics->openxr->ready();

    graphics->quickRenderer->initialize(graphics->glContext);
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

    //Create acquire information
    XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO, nullptr};

    //Create empty swapchain wait info
    XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, nullptr};

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

    emit startedFrame();
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

    copyFrame(0);
    copyFrame(1);

    emit renderedFrame();
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
	qDebug() << "FPS: " << fps << endl;

    emit frameEnded();
}


//Vulkan shortcuts

void OpenXRFrame::copyFrame(uint i) {
    VkImage image = graphics->vulkanImages[i][0];

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(3);

    VkImageSubresourceRange range;
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    range.baseMipLevel = 0;
    range.levelCount = 1;

    VkImageMemoryBarrier layoutTransition = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        graphics->openxr->vulkan->queueFamilyIndex,
        graphics->openxr->vulkan->queueFamilyIndex,
        image,
        range
    };

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &layoutTransition);

    const int align_mod = memRequirements.size % memRequirements.alignment;
    const int aligned_size = ((memRequirements.size % memRequirements.alignment) == 0)
                             ? memRequirements.size
                             : (memRequirements.size + memRequirements.alignment - align_mod);

    VkBufferImageCopy region = {};
    region.bufferOffset = align_mod+(graphics->totalSize.width()*2*i);
    region.bufferRowLength = graphics->totalSize.width();
    region.bufferImageHeight = graphics->totalSize.height();

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = VkOffset3D{
        0,0,0
    };
    region.imageExtent = VkExtent3D{
        static_cast<uint32_t>(graphics->eyeRects[i].width()),
        static_cast<uint32_t>(graphics->eyeRects[i].height()),
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        stagingBuffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    layoutTransition.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    layoutTransition.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    layoutTransition.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    layoutTransition.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &layoutTransition);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr
    };
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer cmdBuffers[1] = {commandBuffer};
    submitInfo.pCommandBuffers = cmdBuffers;

    vkQueueSubmit(*graphics->openxr->vulkan->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*graphics->openxr->vulkan->queue);
}

VkCommandBuffer OpenXRFrame::beginSingleTimeCommands(uint32_t count) {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = graphics->openxr->vulkan->pool;
    allocInfo.commandBufferCount = count;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(graphics->openxr->vulkan->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void OpenXRFrame::createEXTBuffers() {
    imageSize = graphics->totalSize.width()*graphics->totalSize.height()*4;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, stagingBuffer, stagingBufferMemory, fd, memRequirements);

    GLint isDedicated = GL_TRUE;

    GLuint glMemObj = 0;

    //glCreateMemoryObjectsEXT(1, &glMemObj);
    (reinterpret_cast<PFNGLCREATEMEMORYOBJECTSEXTPROC>(graphics->glContext->getProcAddress("glCreateMemoryObjectsEXT")))(1, &glMemObj);

    //glImportMemoryFdEXT (glMemObj, imageSize, GL_HANDLE_TYPE_OPAQUE_FD_EXT, fd);
    (reinterpret_cast<PFNGLIMPORTMEMORYFDEXTPROC>(graphics->glContext->getProcAddress("glImportMemoryFdEXT")))(glMemObj, imageSize, GL_HANDLE_TYPE_OPAQUE_FD_EXT, fd);

    glGenTextures(1, &colorTex);

    glBindTexture (GL_TEXTURE_2D, colorTex);

//    glTexStorageMem2DEXT(GL_TEXTURE_2D, 1, GL_RGBA8, graphics->totalSize.width(), graphics->totalSize.height(), glMemObj, 0);
    (reinterpret_cast<PFNGLTEXSTORAGEMEM2DEXTPROC>(graphics->glContext->getProcAddress("glTexStorageMem2DEXT")))(GL_TEXTURE_2D, 1, GL_RGBA8, graphics->totalSize.width(), graphics->totalSize.height(), glMemObj, 0);

    glFinish();
}

void OpenXRFrame::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, int &fd, VkMemoryRequirements& memRequirements) {
    VkExternalMemoryBufferCreateInfo externalBufferInfo = {
        VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO,
        nullptr,
        VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT
    };

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = &externalBufferInfo;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    bufferInfo.pQueueFamilyIndices = &graphics->openxr->vulkan->queueFamilyIndex;

    if (vkCreateBuffer(graphics->openxr->vulkan->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    vkGetBufferMemoryRequirements(graphics->openxr->vulkan->device, buffer, &memRequirements);

    const int align_mod = memRequirements.size % memRequirements.alignment;
    const int aligned_size = ((memRequirements.size % memRequirements.alignment) == 0)
                             ? memRequirements.size
                             : (memRequirements.size + memRequirements.alignment - align_mod);


    VkMemoryDedicatedAllocateInfoKHR dedicated_memory_info = {
        VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR,
        nullptr,
        VK_NULL_HANDLE,
        buffer
    };

    VkMemoryAllocateInfo allocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        &dedicated_memory_info,
        memRequirements.size,
        findMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, properties)
    };
    allocInfo.allocationSize = memRequirements.size;

    VkResult result = vkAllocateMemory(graphics->openxr->vulkan->device, &allocInfo, nullptr, &bufferMemory);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    result = vkBindBufferMemory(graphics->openxr->vulkan->device, buffer, bufferMemory, 0);

    PFN_vkGetMemoryFdKHR extVkGetMemoryFdKHR = (PFN_vkGetMemoryFdKHR) vkGetDeviceProcAddr (graphics->openxr->vulkan->device, "vkGetMemoryFdKHR");
    VkMemoryGetFdInfoKHR vulkanFdInfo = {
        VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR,
        nullptr,
        bufferMemory,
        VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT_KHR
    };

    extVkGetMemoryFdKHR(graphics->openxr->vulkan->device, &vulkanFdInfo, &fd);
}

uint32_t OpenXRFrame::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(graphics->openxr->vulkan->physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
}

}
